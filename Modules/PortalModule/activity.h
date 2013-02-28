#ifndef ACTIVITY_H
#define ACTIVITY_H

#include <QObject>

class Activity : public QObject
{
    Q_OBJECT
public:
    explicit Activity(QObject *parent = 0);

    QString user_name;
    QString content;
    QString date_created;
    int activity_type;
    int group_id;
    int object_id;

    enum ActivitiesTypes
    {
        RECOMMENDATION,
        RATING,
        FILE
    };

signals:
    
public slots:
    
};

#endif // ACTIVITY_H
