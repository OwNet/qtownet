#ifndef OWNETUPDATER_H
#define OWNETUPDATER_H

#include <QObject>
#include <QVariantMap>

class OwNetClient;
class OwNetCloudServer;

class OwNetUpdater : public QObject
{
    Q_OBJECT
public:
    explicit OwNetUpdater(OwNetClient *, OwNetCloudServer *, QObject *parent = 0);
    void checkForUpdates();
    
signals:
    
public slots:
    void updateMetadataReceived(QVariantMap);
    void packageDataReceived(QString fileName, QByteArray data);

private:
    OwNetClient *m_client;
    OwNetCloudServer *m_cloudServer;
};

#endif // OWNETUPDATER_H
