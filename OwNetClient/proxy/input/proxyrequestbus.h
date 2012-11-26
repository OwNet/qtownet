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

    QByteArray* callModule(IRequest *req);
    void setHttpStatus(int code, const QString &description);

    static void registerModule(RequestRouter *router);

protected:
    void readRequest();

private:
    ProxyRequest *m_request;

    static QMap<QString, RequestRouter*> *m_routes;
};

#endif // REQUESTBUS_H
