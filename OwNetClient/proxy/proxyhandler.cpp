#include "proxyhandler.h"

#include "messagehelper.h"
#include "proxyresponseoutputwriter.h"
#include "proxyhandlersession.h"
#include "sockethandler.h"
#include "requestreader.h"
#include "staticreader.h"
#include "servicereader.h"
#include "proxyrequest.h"
#include "webreader.h"
#include "directwebreader.h"
#include "webdownloadsmanager.h"

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
        StaticReader(m_socketHandler, request).read();
        m_socketHandler->proxyHandlerFinished(this);
    } else if (request->isLocalRequest()) {
        ServiceReader(m_socketHandler, request).read();
        m_socketHandler->proxyHandlerFinished(this);
    } else {
        m_proxyHandlerSession = new ProxyHandlerSession(this);
        connect(m_proxyHandlerSession, SIGNAL(allFinished()), this, SLOT(proxyHandlerSessionFinished()), Qt::QueuedConnection);

        if (WebDownloadsManager::instance()->containsCacheExceptionFor(request->url()))
            (new DirectWebReader(socketHandler, request, m_proxyHandlerSession))->read();
        else
            (new WebReader(socketHandler, request, m_proxyHandlerSession))->read();
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
