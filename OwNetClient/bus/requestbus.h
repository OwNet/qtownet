#ifndef REQUESTBUS_H
#define REQUESTBUS_H

#include "proxyinputobject.h"

class ProxyRequest;
class QIODevice;

class RequestBus : public ProxyInputObject
{
    Q_OBJECT

public:
    RequestBus(ProxyRequest *request, QObject *parent = 0);

    void readRequest();

    const QString httpStatusCode() { return QString::number(200); }
    const QString httpStatusDescription() { return "OK"; }

private:
    ProxyRequest *m_request;
};

#endif // REQUESTBUS_H
