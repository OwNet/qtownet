#ifndef REQUESTBUS_H
#define REQUESTBUS_H

#include "proxyinputobject.h"
#include "ibus.h"

class RequestRouter;
class ProxyRequest;
class QIODevice;
class IResponse;

class ProxyRequestBus : public ProxyInputObject
{
    Q_OBJECT

public:
    ProxyRequestBus(ProxyRequest *request, QObject *parent = 0);

protected:
    void readRequest();

private:
    QByteArray* processResponse(IResponse *response);

    ProxyRequest *m_request;
};

#endif // REQUESTBUS_H
