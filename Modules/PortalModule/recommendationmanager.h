#ifndef RECOMMENDATIONMANAGER_H
#define RECOMMENDATIONMANAGER_H

#include <QObject>
#include "activitymanager.h"
#include "activity.h"
#include "iproxyconnection.h"

class RecommendationManager : public QObject
{
    Q_OBJECT
public:
    explicit RecommendationManager(IProxyConnection *proxyConnection, QObject *parent = 0);

    IResponse::Status createRecomm(IRequest *req, QString curUser_id, QVariantMap &error);
    IResponse::Status editRecomm(IRequest *req, QString uid, QString curUser_id, QVariantMap &error);
    IResponse::Status showRecomm(IRequest *req, QString id, QString curUser_id, QVariantMap &recommendation, QVariantMap &error);
    IResponse::Status deleteRecomm(IRequest *req, QString uid, QString curUser_id, QVariantMap &error);

private:
    ActivityManager *m_activityManager;
    IProxyConnection *m_proxyConnection;
    
signals:
    
public slots:
    
};

#endif // RECOMMENDATIONMANAGER_H
