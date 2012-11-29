#ifndef SERVICE_H
#define SERVICE_H

#include <QObject>

#include "irestservice.h"

class IProxyConnection;

class MessagesService : public QObject, public IRestService
{
    Q_OBJECT
public:
    explicit MessagesService(IProxyConnection *proxyConnection, QObject *parent = 0);

    QString name() const { return "messages"; }

    QVariant *create(IBus *bus, IRequest *req);
    // QVariant *show(IBus *bus, IRequest *req);
    QVariant *index(IBus *bus, IRequest *);
    QVariant *del(IBus *bus, IRequest *);

private:
    IProxyConnection *m_proxyConnection;
};

#endif // SERVICE_H
