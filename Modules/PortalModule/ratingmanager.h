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

    IResponse::Status createRating(QString userId, QString uri, int value, QVariantMap &error);
    IResponse::Status showRating(QString uid, QVariantMap &rating, QVariantMap &error);
    IResponse::Status editRating(QString uid, uint userId, int value, QVariantMap &error);
    IResponse::Status deleteRating(QString uid, uint userId, QVariantMap &error);

    IResponse::Status showAllPageRatings(QString uri, QVariantList &ratings, QVariantMap &error);
    IResponse::Status showAllUserRatings(uint userId, QVariantList &ratings, QVariantMap &error);
    IResponse::Status showPageStats(QString uri, uint userId, QVariantMap &stats, QVariantMap &error);

private:
    ActivityManager *m_activityManager;
    IProxyConnection *m_proxyConnection;


signals:
    
public slots:
    
};

#endif // RATINGMANAGER_H
