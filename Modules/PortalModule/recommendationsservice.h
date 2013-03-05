#ifndef RECOMMENDATIONSSERVICE_H
#define RECOMMENDATIONSSERVICE_H

#include <QObject>

#include "irestservice.h"
#include "recommendationmanager.h"

class IProxyConnection;

class RecommendationsService : public QObject, public IRestService
{
    Q_OBJECT
public:
    explicit RecommendationsService(IProxyConnection *proxyConnection, QObject *parent = 0);

    void init(IRouter* router);

    QString name() const { return "recommendations"; }

     IResponse *create (IRequest *req);
     IResponse *show (IRequest *req, uint id);
     //IResponse *index(IRequest *req);
     IResponse *edit (IRequest *, uint id);
     IResponse *del (IRequest *req, uint id);

private:
    IProxyConnection *m_proxyConnection;
    RecommendationManager *m_recommendationManager;
};

#endif // RECOMMENDATIONSSERVICE_H
