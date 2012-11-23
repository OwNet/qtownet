#include "proxyhandler.h"

#include "messagehelper.h"
#include "proxyrequest.h"
#include "proxywebinputobject.h"
#include "proxystaticinputobject.h"
#include "proxyrequestbus.h"
#include "proxydownloads.h"
#include "proxydownload.h"
#include "proxysocketoutputwriter.h"
#include "proxyhandlersession.h"

#include <QRegExp>
#include <QWidget>
#include <QDateTime>
#include <QNetworkAccessManager>
#include <QSemaphore>
#include <QTimer>
#include <QApplication>

ProxyHandler::ProxyHandler(int handlerId, QObject *parent)
    : QObject(parent),
      m_handlerId(handlerId),
      m_isActive(false),
      m_proxyHandlerSession(NULL),
      m_timeoutTimer(NULL)
{
    m_openSemaphore = new QSemaphore(1);
    connect(this, SIGNAL(start()), this, SLOT(handleRequest()));
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

void ProxyHandler::dispose()
{
    emit disposeThread();
}

void ProxyHandler::handleRequest()
{
    m_proxyHandlerSession = new ProxyHandlerSession(this);
    connect(m_proxyHandlerSession, SIGNAL(allFinished()), this, SLOT(proxyHandlerSessionFinished()));

    m_timeoutTimer = new QTimer(m_proxyHandlerSession);
    connect(m_timeoutTimer, SIGNAL(timeout()), this, SLOT(requestTimeout()));

    ProxySocketOutputWriter *socketOutputWriter = new ProxySocketOutputWriter(m_socketDescriptor, m_proxyHandlerSession);
    connect(socketOutputWriter, SIGNAL(iAmActive()), this, SLOT(restartTimeout()));

    socketOutputWriter->startDownload();
    m_timeoutTimer->start(Timeout);
}

void ProxyHandler::requestTimeout()
{
    m_timeoutTimer->stop();

    if (m_proxyHandlerSession)
        m_proxyHandlerSession->forceQuitAll();
}

void ProxyHandler::restartTimeout()
{
    if (m_timeoutTimer) {
        m_timeoutTimer->stop();
        m_timeoutTimer->start();
    }
}

void ProxyHandler::finishHandling()
{
    if (m_proxyHandlerSession)
        m_proxyHandlerSession->forceQuitAll();
}

void ProxyHandler::proxyHandlerSessionFinished()
{
    if (m_timeoutTimer) {
        m_timeoutTimer->stop();
        m_timeoutTimer = NULL;
    }

    if (m_proxyHandlerSession) {
        m_proxyHandlerSession->deleteLater();
        m_proxyHandlerSession = NULL;

        m_openSemaphore->release();
        emit requestFinished(this);
    }
    m_isActive = false;
}
