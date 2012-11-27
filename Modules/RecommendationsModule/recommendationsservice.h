#ifndef RECOMMENDATIONSSERVICE_H
#define RECOMMENDATIONSSERVICE_H

#include <QObject>

#include "irestservice.h"

class IProxyConnection;

class RecommendationsService : public QObject, public IRestService
{
    Q_OBJECT
public:
    explicit RecommendationsService(IProxyConnection *proxyConnection, QObject *parent = 0);

    QString name() const { return "recommendations"; }

    QVariant *create(IBus *bus, IRequest *req);
    // QVariant *show(IBus *bus, IRequest *req);
    QVariant *index(IBus *bus, IRequest *);
    QVariant *edit(IBus *bus, IRequest *);
    QVariant *del(IBus *bus, IRequest *);

private:
    IProxyConnection *m_proxyConnection;
};

#endif // RECOMMENDATIONSSERVICE_H
