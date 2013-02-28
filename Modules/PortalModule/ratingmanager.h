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

    bool createRating();

    
private:
    ActivityManager *m_activityManager;
    IProxyConnection *m_proxyConnection;


signals:
    
public slots:
    
};

#endif // RATINGMANAGER_H
