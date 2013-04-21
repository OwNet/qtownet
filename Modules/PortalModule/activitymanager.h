#ifndef ACTIVITYMANAGER_H
#define ACTIVITYMANAGER_H

#include <QObject>
#include "activity.h"
#include <QVariant>

#include "iproxyconnection.h"


class ActivityManager : public QObject
{
    Q_OBJECT
public:
    explicit ActivityManager(IProxyConnection *proxyConnection, QObject *parent = 0);
    
    bool createActivity(Activity &ac);
    QVariantList getActivities(bool *ok, QVariantMap &error, IRequest *req);
    bool deleteActivity(const QString &objectId);
    bool editActivity(const QString &objectId, QString content);
    int pagesCount(IRequest *req);


    QVariantList getUsersActivities(bool *ok, QVariantMap &error, IRequest *req);
    int usersPagesCount(IRequest *req);

private:
    IProxyConnection *m_proxyConnection;

signals:
    
public slots:
    
};

#endif // ACTIVITYMANAGER_H
