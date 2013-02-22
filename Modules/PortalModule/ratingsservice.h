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

     IResponse *create(IRequest *req);
     IResponse *show( IRequest *req);
     IResponse *index(IRequest *req);
   // QVariant *edit(IBus *bus, IRequest *);
     IResponse *del(IRequest *req);

private:
    IProxyConnection *m_proxyConnection;
};

#endif // RATINGSSERVICE_H
