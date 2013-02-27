#ifndef ACTIVITYMANAGER_H
#define ACTIVITYMANAGER_H

#include <QObject>
#include "activity.h"
#include <QVariant>


class ActivityManager : public QObject
{
    Q_OBJECT
public:
    explicit ActivityManager(QObject *parent = 0);
    
    static bool createActivity(Activity ac);

    static QVariantList getActivities(bool *ok);


signals:
    
public slots:
    
};

#endif // ACTIVITYMANAGER_H
