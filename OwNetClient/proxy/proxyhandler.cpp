#include "proxyhandler.h"

#include "messagehelper.h"
#include "proxyrequest.h"
#include "proxywebinputobject.h"
#include "proxystaticinputobject.h"
#include "proxyrequestbus.h"
#include "proxydownloads.h"
#include "proxydownload.h"
#include "proxysocketoutputwriter.h"

#include <QRegExp>
#include <QWidget>
#include <QDateTime>
#include <QNetworkAccessManager>
#include <QSemaphore>
#include <QTimer>
#include <QApplication>

ProxyHandler::ProxyHandler(int handlerId, QObject *parent)
    : QObject(parent), m_handlerId(handlerId), m_isActive(false), m_timeoutTimer(NULL)
{
    m_openSemaphore = new QSemaphore(1);
    connect(this, SIGNAL(start()), this, SLOT(handleRequest()));
    connect(this, SIGNAL(finish()), this, SLOT(finishHandling()));
}

ProxyHandler::~ProxyHandler()
{
    delete m_openSemaphore;
}

void ProxyHandler::setDescriptorAndStart(int desc) {
    m_isActive = true;
    m_openSemaphore->acquire();
    m_socketDescriptor = desc;
    emit start();
}

void ProxyHandler::triggerFinish()
{
    emit finish();
}

int ProxyHandler::registerDependentObject()
{
    QMutexLocker mutexLocker(&m_dependentObjectsMutex);

    int objectId = m_lastDependentObjectId++;
    m_dependentObjects.append(objectId);
    return objectId;
}

void ProxyHandler::deregisterDependentObject(int objectId)
{
    QMutexLocker mutexLocker(&m_dependentObjectsMutex);

    m_dependentObjects.removeAll(objectId);
    if (!m_dependentObjects.count() && !m_isActive)
        emit canBeDisposed(this);
}

bool ProxyHandler::hasDependentObjects()
{
    QMutexLocker mutexLocker(&m_dependentObjectsMutex);

    return m_dependentObjects.count() > 0;
}

void ProxyHandler::handleRequest()
{
    m_timeoutTimer = new QTimer(this);
    connect(m_timeoutTimer, SIGNAL(timeout()), this, SLOT(requestTimeout()));

    m_socketOutputWriter = new ProxySocketOutputWriter(m_socketDescriptor, this);
    connect(m_socketOutputWriter, SIGNAL(finished()), this, SLOT(downloadFinished()));
    connect(m_socketOutputWriter, SIGNAL(iAmActive()), this, SLOT(restartTimeout()));

    m_socketOutputWriter->startDownload();
    m_timeoutTimer->start(Timeout);
}

void ProxyHandler::requestTimeout()
{
    if (m_socketOutputWriter)
        m_socketOutputWriter->finish();

    finishHandlingRequest();
}

void ProxyHandler::restartTimeout()
{
    if (m_timeoutTimer) {
        m_timeoutTimer->stop();
        m_timeoutTimer->start();
    }
}

void ProxyHandler::finishHandlingRequest()
{
    if (m_timeoutTimer) {
        m_timeoutTimer->stop();
        delete m_timeoutTimer;
        m_timeoutTimer = NULL;
    }

    if (m_socketOutputWriter) {
        qApp->processEvents();
        m_socketOutputWriter->deleteLater();
        m_socketOutputWriter = NULL;

        m_openSemaphore->release();
        emit requestFinished(this);
    }
}

void ProxyHandler::error(QNetworkReply::NetworkError)
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    MessageHelper::debug(reply->errorString());
    finishHandlingRequest();
}

void ProxyHandler::downloadFinished()
{
    finishHandlingRequest();
}

void ProxyHandler::finishHandling()
{
    if (m_socketOutputWriter)
        m_socketOutputWriter->finish();

    finishHandlingRequest();
    emit finished();
    m_isActive = false;
}
