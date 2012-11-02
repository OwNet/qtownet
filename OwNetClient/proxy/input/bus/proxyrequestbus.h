#ifndef REQUESTBUS_H
#define REQUESTBUS_H

#include "proxyinputobject.h"

class ProxyRequest;
class QIODevice;

class ProxyRequestBus : public ProxyInputObject
{
    Q_OBJECT

public:
    ProxyRequestBus(ProxyRequest *request, QObject *parent = 0);

    const QString httpStatusCode() { return QString::number(200); }
    const QString httpStatusDescription() { return "OK"; }

protected:
    void readRequest();

private:
    ProxyRequest *m_request;
};

#endif // REQUESTBUS_H
