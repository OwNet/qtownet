#ifndef OWNETCLOUDSERVER_H
#define OWNETCLOUDSERVER_H

#include <QObject>

class QNetworkAccessManager;

class OwNetCloudServer : public QObject
{
    Q_OBJECT
public:
    explicit OwNetCloudServer(QObject *parent = 0);
    void sendCrashReport(QString);
    
signals:
    
public slots:
    
private:
    QNetworkAccessManager *m_nam;
};

#endif // OWNETCLOUDSERVER_H
