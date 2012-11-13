#ifndef REQUESTBUS_H
#define REQUESTBUS_H

#include "proxyinputobject.h"
#include "imodule.h"
#include "ibus.h"
#include <QByteArray>

class ProxyRequest;
class QIODevice;

class ProxyRequestBus : public ProxyInputObject, public IBus
{
    Q_OBJECT


public:
    ProxyRequestBus(ProxyRequest *request, QObject *parent = 0);

    const QString httpStatusCode() { return QString::number(200); }
    const QString httpStatusDescription() { return "OK"; }

    QByteArray* callModule(ProxyRequest *req);
    void setHttpStatus(int code, const QString &description);

    static void registerModule(IModule *newModule, QString url);

protected:
    void readRequest();

private:
    ProxyRequest *m_request;

    static QMap<QString ,IModule*> *m_routes;
};

#endif // REQUESTBUS_H
