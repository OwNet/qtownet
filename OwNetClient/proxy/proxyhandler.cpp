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

ProxyHandler::ProxyHandler(QObject *parent)
    : QObject(parent)
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
    m_openSemaphore->acquire();
    m_socketDescriptor = desc;
    emit start();
}

void ProxyHandler::triggerFinish()
{
    emit finish();
}

void ProxyHandler::handleRequest()
{
    m_timeoutTimer = new QTimer(this);
    connect(m_timeoutTimer, SIGNAL(timeout()), this, SLOT(requestTimeout()));

    m_socketOutputWriter = new ProxySocketOutputWriter(m_socketDescriptor);
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
    m_timeoutTimer->stop();
    m_timeoutTimer->start();
}

void ProxyHandler::finishHandlingRequest()
{
    m_timeoutTimer->stop();
    delete m_timeoutTimer;
    m_timeoutTimer = NULL;

    if (m_socketOutputWriter) {
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
}
