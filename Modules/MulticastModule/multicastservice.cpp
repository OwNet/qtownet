#include "multicastservice.h"
#include "irouter.h"

MulticastService::MulticastService(IProxyConnection *proxyConnection, MulticastProtocol *protocol,
                                   QObject *parent) :
    QObject(parent),
    m_protocol(protocol),
    m_proxyConnection(proxyConnection)
{
}

void MulticastService::init(IRouter *router)
{
    router->addRoute("/initialized")
            ->on(IRequest::POST, ROUTE(initialized));
}

/**
 * @brief Set initialized timestamp for multicast.
 * @param request
 * @return
 */
IResponse *MulticastService::initialized(IRequest *request)
{
    m_protocol->initialized();

    return request->response(IResponse::OK);
}
