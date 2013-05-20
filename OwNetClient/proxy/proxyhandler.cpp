#include "proxyhandler.h"

#include "messagehelper.h"
#include "proxyresponseoutputwriter.h"
#include "proxyhandlersession.h"
#include "sockethandler.h"
#include "requestreader.h"
#include "proxystaticreader.h"
#include "proxyservicereader.h"
#include "proxyrequest.h"
#include "proxywebreader.h"

#include <QDateTime>

ProxyHandler::ProxyHandler(QObject *parent)
    : QObject(parent),
      m_proxyHandlerSession(NULL),
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

        (new ProxyWebReader(socketHandler, request, m_proxyHandlerSession))->read();
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
    if (m_proxyHandlerSession) {
        m_proxyHandlerSession->deleteLater();
        m_proxyHandlerSession = NULL;

        m_socketHandler->proxyHandlerFinished(this);
    }
}
