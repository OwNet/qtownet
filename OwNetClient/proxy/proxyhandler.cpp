#include "proxyhandler.h"

#include "messagehelper.h"
#include "proxyresponseoutputwriter.h"
#include "proxyhandlersession.h"
#include "httpresponse.h"

#include <QDateTime>
#include <QTimer>

ProxyHandler::ProxyHandler(QObject *parent)
    : QObject(parent),
      m_proxyHandlerSession(NULL),
      m_timeoutTimer(NULL)
{
}

void ProxyHandler::service(HttpRequest *request, HttpResponse *response) {
    m_request = request;
    m_response = response;

    m_proxyHandlerSession = new ProxyHandlerSession(this);
    connect(m_proxyHandlerSession, SIGNAL(allFinished()), this, SLOT(proxyHandlerSessionFinished()));

    m_timeoutTimer = new QTimer(m_proxyHandlerSession);
    connect(m_timeoutTimer, SIGNAL(timeout()), this, SLOT(requestTimeout()));

    ProxyResponseOutputWriter *responseOutputWriter = new ProxyResponseOutputWriter(m_request, m_response, m_proxyHandlerSession);
    connect(responseOutputWriter, SIGNAL(iAmActive()), this, SLOT(restartTimeout()));

    responseOutputWriter->startDownload();
    m_timeoutTimer->start(Timeout);
}

/**
 * @brief Request timed out.
 */
void ProxyHandler::requestTimeout()
{
    MessageHelper::debug("ProxyHandler timeout.");
    m_timeoutTimer->stop();

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

        emit m_response->finished();
    }
}
