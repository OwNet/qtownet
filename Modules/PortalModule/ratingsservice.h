#ifndef RATINGSSERVICE_H
#define RATINGSSERVICE_H

#include <QObject>

#include "irestservice.h"
#include "ratingmanager.h"


class RatingsService : public QObject, public IRestService
{
    Q_OBJECT
public:
    explicit RatingsService(IProxyConnection *proxyConnection, QObject *parent = 0);

    void init(IRouter* router);

    QString name() const { return "ratings"; }

    IResponse *create(IRequest *req);
    IResponse *show( IRequest *req);
    IResponse *del(IRequest *req);

private:

    IProxyConnection *m_proxyConnection;

    RatingManager *m_ratingManager;
};

#endif // RATINGSSERVICE_H
