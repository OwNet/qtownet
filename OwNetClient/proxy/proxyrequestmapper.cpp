#include "proxyrequestmapper.h"

#include "proxyhandler.h"

#include <QEventLoop>

ProxyRequestMapper::ProxyRequestMapper(QObject* parent)
    : HttpRequestHandler(parent)
{
}

/**
  Dispatch a request to a controller.
  @param request The received HTTP request
  @param response Must be used to return the response
*/
void ProxyRequestMapper::service(HttpRequest *request, HttpResponse *response)
{
    ProxyHandler *handler = new ProxyHandler(response);
    connect(handler, SIGNAL(disposeThread()), response, SIGNAL(finished()));
    handler->setRequestAndResponseAndStart(request, response);
}
