#include "proxyhandler.h"

#include "messagehelper.h"
#include "proxyresponseoutputwriter.h"
#include "proxyhandlersession.h"
#include "sockethandler.h"
#include "requestreader.h"
#include "proxystaticreader.h"
#include "proxyservicereader.h"
#include "proxyrequest.h"

#include <QDateTime>
#include <QTimer>

ProxyHandler::ProxyHandler(QObject *parent)
    : QObject(parent),
      m_proxyHandlerSession(NULL),
      m_timeoutTimer(NULL),
      m_socketHandler(NULL)
{
}

void ProxyHandler::service(SocketHandler *socketHandler) {
    m_socketHandler = socketHandler;

    ProxyRequest *request = new ProxyRequest(m_socketHandler->requestReader(), m_proxyHandlerSession);
    if (request->isStaticResourceRequest()) {
        ProxyStaticReader(m_socketHandler, request).read();
        m_socketHandler->proxyHandlerFinished(this);
    } else if (request->isLocalRequest()) {
        ProxyServiceReader(m_socketHandler, request).read();
        m_socketHandler->proxyHandlerFinished(this);
    } else {
        m_proxyHandlerSession = new ProxyHandlerSession(this);
        connect(m_proxyHandlerSession, SIGNAL(allFinished()), this, SLOT(proxyHandlerSessionFinished()));

        m_timeoutTimer = new QTimer(m_proxyHandlerSession);
        connect(m_timeoutTimer, SIGNAL(timeout()), this, SLOT(requestTimeout()));

        ProxyResponseOutputWriter *responseOutputWriter = new ProxyResponseOutputWriter(socketHandler, m_proxyHandlerSession);
        connect(responseOutputWriter, SIGNAL(iAmActive()), this, SLOT(restartTimeout()));

        responseOutputWriter->startDownload(request);
        m_timeoutTimer->start(Timeout);
    }
}

/**
 * @brief Request timed out.
 */
void ProxyHandler::requestTimeout()
{
    MessageHelper::debug(QString("ProxyHandler timeout - %1").arg(m_socketHandler->requestReader()->url()));
    if (m_timeoutTimer) {
        m_timeoutTimer->stop();
        m_timeoutTimer = NULL;
    }

    if (m_proxyHandlerSession)
        m_proxyHandlerSession->forceQuitAll();
}

/**
 * @brief Restart timeot timer after SocketOutputWriter triggers iAmReady signal.
 */
void ProxyHandler::restartTimeout()
{
    if (m_timeoutTimer) {
        m_timeoutTimer->stop();
        m_timeoutTimer->start();
    }
}

/**
 * @brief Triggered from outside to force quit the current download.
 */
void ProxyHandler::finishHandling()
{
    if (m_proxyHandlerSession)
        m_proxyHandlerSession->forceQuitAll();
}

/**
 * @brief The current download and all dependent objects finished, disposes the current download
 * and emits requestFinished signal.
 */
void ProxyHandler::proxyHandlerSessionFinished()
{
    if (m_timeoutTimer) {
        m_timeoutTimer->stop();
        m_timeoutTimer = NULL;
    }

    if (m_proxyHandlerSession) {
        m_proxyHandlerSession->deleteLater();
        m_proxyHandlerSession = NULL;

        m_socketHandler->proxyHandlerFinished(this);
    }
}
