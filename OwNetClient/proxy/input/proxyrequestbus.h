#ifndef REQUESTBUS_H
#define REQUESTBUS_H

#include "proxyinputobject.h"

class RequestRouter;
class ProxyRequest;
class QIODevice;
class IResponse;

class ProxyRequestBus : public ProxyInputObject
{
    Q_OBJECT

public:
    ProxyRequestBus(ProxyRequest *request, QObject *parent = 0);

    InputType inputType() const { return LocalService; }
    bool addContentLengthHeader() const { return true; }

protected:
    void readRequest();

private:
    QByteArray* processResponse(IResponse *response);

    ProxyRequest *m_request;
};

#endif // REQUESTBUS_H
