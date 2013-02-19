#ifndef REQUESTMAPPER_H
#define REQUESTMAPPER_H

#include "httprequesthandler.h"

/**
  The request mapper dispatches incoming HTTP requests to controller classes
  depending on the requested path.
*/

class ProxyRequestMapper : public HttpRequestHandler {

    Q_OBJECT
public:
    ProxyRequestMapper(QObject* parent=0);

    void service(HttpRequest& request, HttpResponse& response);
};

#endif // REQUESTMAPPER_H
