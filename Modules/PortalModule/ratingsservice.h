#ifndef RATINGSSERVICE_H
#define RATINGSSERVICE_H

#include <QObject>

#include "iuidrestservice.h"
#include "ratingmanager.h"


class RatingsService : public QObject, public IUidRestService
{
    Q_OBJECT
public:
    explicit RatingsService(IProxyConnection *proxyConnection, QObject *parent = 0);

    void init(IRouter* router);

    QString name() const { return "ratings"; }

    IResponse *create(IRequest *req);
    //IResponse *index(IRequest *req);
    IResponse *show(IRequest *req, const QString &textId);
    IResponse *edit(IRequest *req, const QString &textId);
    IResponse *del(IRequest *req, const QString &textId);

    IResponse *showPageStats(IRequest *req);    
    IResponse *showAllPageRatings(IRequest *req);


private:

    IProxyConnection *m_proxyConnection;

    RatingManager *m_ratingManager;
};

#endif // RATINGSSERVICE_H
