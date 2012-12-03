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

    QVariant *callModule(IRequest *req);
    void setHttpStatus(int code, const QString &description);

    QString contentType() const { return ProxyInputObject::contentType(); }
    void setContentType(const QString &value);

    static void registerModule(RequestRouter *router);

protected:
    void readRequest();

private:
    ProxyRequest *m_request;

    static QMap<QString, RequestRouter*> *m_routes;
};

#endif // REQUESTBUS_H
