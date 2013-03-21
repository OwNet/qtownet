#ifndef RECOMMENDATIONSSERVICE_H
#define RECOMMENDATIONSSERVICE_H

#include <QObject>

#include "iuidrestservice.h"
#include "recommendationmanager.h"

class IProxyConnection;

class RecommendationsService : public QObject, public IUidRestService
{
    Q_OBJECT
public:
    explicit RecommendationsService(IProxyConnection *proxyConnection, QObject *parent = 0);

    void init(IRouter* router);

    QString name() const { return "recommendations"; }

     IResponse *create (IRequest *req);
     IResponse *show (IRequest *req, const QString &id);
     //IResponse *index(IRequest *req);
     IResponse *edit (IRequest *, const QString &id);
     IResponse *del (IRequest *req, const QString &id);

private:
    IProxyConnection *m_proxyConnection;
    RecommendationManager *m_recommendationManager;
};

#endif // RECOMMENDATIONSSERVICE_H
