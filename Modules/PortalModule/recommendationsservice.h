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

    void init(IRouter* router);

    QString name() const { return "recommendations"; }

     IResponse *create(IRequest *req);
     //IResponse *show( IRequest *req);
     IResponse *index(IRequest *req);
     IResponse *edit(IRequest *);
     IResponse *del(IRequest *req);

private:
    IProxyConnection *m_proxyConnection;
};

#endif // RECOMMENDATIONSSERVICE_H
