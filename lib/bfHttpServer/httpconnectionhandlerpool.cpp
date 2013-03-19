#include "httpconnectionhandlerpool.h"

HttpConnectionHandlerPool::HttpConnectionHandlerPool(QSettings* settings, HttpRequestHandler* requestHandler)
    : QObject()
{
    Q_ASSERT(settings!=0);
    this->settings=settings;
    this->requestHandler=requestHandler;
}

HttpConnectionHandler* HttpConnectionHandlerPool::getConnectionHandler() {   
    HttpConnectionHandler *handler = new HttpConnectionHandler(settings, requestHandler);
    return handler;
}
