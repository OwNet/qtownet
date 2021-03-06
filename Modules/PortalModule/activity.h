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
    QString group_id;
    QString user_id;
    QString object_id;
    QString gender;

    enum ActivitiesTypes
    {
        RECOMMENDATION,
        RATING,
        MESSAGE,
        FILE
    };

signals:
    
public slots:
    
};

#endif // ACTIVITY_H
