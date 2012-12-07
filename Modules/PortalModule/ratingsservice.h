#ifndef RATINGSSERVICE_H
#define RATINGSSERVICE_H

#include <QObject>

#include "irestservice.h"

class IProxyConnection;

class RatingsService : public QObject, public IRestService
{
    Q_OBJECT
public:
    explicit RatingsService(IProxyConnection *proxyConnection, QObject *parent = 0);

    QString name() const { return "ratings"; }

    QVariant *create(IBus *bus, IRequest *req);
    QVariant *show(IBus *bus, IRequest *req);
    QVariant *index(IBus *bus, IRequest *);
   // QVariant *edit(IBus *bus, IRequest *);
    QVariant *del(IBus *bus, IRequest *);

private:
    IProxyConnection *m_proxyConnection;
};

#endif // RATINGSSERVICE_H
