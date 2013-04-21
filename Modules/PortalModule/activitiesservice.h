#ifndef ACTIVITIESSERVICE_H
#define ACTIVITIESSERVICE_H

#include <QObject>

#include "iuidrestservice.h"
#include "activitymanager.h"

class ActivitiesService : public QObject, public IUidRestService
{
    Q_OBJECT
public:
    explicit ActivitiesService(IProxyConnection *proxyConnection, QObject *parent = 0);

    void init(IRouter* router);

    QString name() const { return "activities"; }

    IResponse *index(IRequest *req);

    IResponse *pagesCount(IRequest *req);

    IResponse *usersPagesCount(IRequest *req);
    IResponse *getUsersActivities(IRequest *req);
private:

    ActivityManager *m_activityManager;

    IProxyConnection *m_proxyConnection;

};

#endif // ACTIVITIESSERVICE_H
