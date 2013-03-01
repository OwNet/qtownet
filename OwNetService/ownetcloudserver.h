#ifndef OWNETCLOUDSERVER_H
#define OWNETCLOUDSERVER_H

#include <QObject>

class OwNetCloudServer : public QObject
{
    Q_OBJECT
public:
    explicit OwNetCloudServer(QObject *parent = 0);
    void sendCrashReport(QString);
    
signals:
    
public slots:
    
};

#endif // OWNETCLOUDSERVER_H
