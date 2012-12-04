#ifndef REQUESTBUS_H
#define REQUESTBUS_H

#include "proxyinputobject.h"
#include "ibus.h"

class RequestRouter;
class ProxyRequest;
class QIODevice;

class ProxyRequestBus : public ProxyInputObject, public IBus
{
    Q_OBJECT

public:
    ProxyRequestBus(ProxyRequest *request, QObject *parent = 0);

    const QString httpStatusCode() { return QString::number(200); }
    const QString httpStatusDescription() { return "OK"; }

    void setHttpStatus(int code, const QString &description);

    QString contentType() const { return ProxyInputObject::contentType(); }
    void setContentType(const QString &value);

protected:
    void readRequest();

private:
    ProxyRequest *m_request;
};

#endif // REQUESTBUS_H
