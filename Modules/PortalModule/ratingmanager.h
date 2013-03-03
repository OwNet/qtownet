#ifndef RATINGMANAGER_H
#define RATINGMANAGER_H

#include <QObject>

#include "activitymanager.h"
#include "activity.h"
#include "iproxyconnection.h"

class RatingManager : public QObject
{
    Q_OBJECT
public:
    explicit RatingManager(IProxyConnection *proxyConnection, QObject *parent = 0);

    IResponse::Status createRating(IRequest *req, QString curUser_id, QVariantMap &error);
    IResponse::Status showRating(IRequest *req, QVariantList &ratings, QVariantMap &error);
    IResponse::Status deleteRating(IRequest *req, QString uid, QString curUser_id, QVariantMap &error);

    
private:
    ActivityManager *m_activityManager;
    IProxyConnection *m_proxyConnection;


signals:
    
public slots:
    
};

#endif // RATINGMANAGER_H
