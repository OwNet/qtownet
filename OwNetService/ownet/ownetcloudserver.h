#ifndef OWNETCLOUDSERVER_H
#define OWNETCLOUDSERVER_H

#include <QObject>
#include <QVariantMap>

class QNetworkAccessManager;
class QNetworkReply;

class OwNetCloudServer : public QObject
{
    Q_OBJECT
public:
    explicit OwNetCloudServer(QObject *parent = 0);
    void sendCrashReport(QString);
    void checkUpdateMetadata();
    void downloadUpdatePackage(QString);
    
signals:
    void updateMetadataReceived(QVariantMap);
    void packageDataReceived(QString fileName, QByteArray data);
public slots:
    void updateMetadataReceived(QNetworkReply *);
    void packageDataReceived(QNetworkReply *);
    
private:
};

#endif // OWNETCLOUDSERVER_H
