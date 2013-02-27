#ifndef ACTIVITIESSERVICE_H
#define ACTIVITIESSERVICE_H

#include <QObject>

#include "irestservice.h"
#include "activitymanager.h"

class IProxyConnection;

class ActivitiesService : public QObject, public IRestService
{
    Q_OBJECT
public:
    explicit ActivitiesService(IProxyConnection *proxyConnection, QObject *parent = 0);

    void init(IRouter* router);

    QString name() const { return "activities"; }

    IResponse *index(IRequest *req);


private:
    enum ActivitiesTypes { RECOMMENDATION, RATING, FILE };

    IProxyConnection *m_proxyConnection;

};

#endif // ACTIVITIESSERVICE_H
