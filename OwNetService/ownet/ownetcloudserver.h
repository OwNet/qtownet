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
    
signals:
    void updateMetadataReceived(QVariantMap);
public slots:
    void updateMetadataReceived(QNetworkReply *);
    
private:
    QNetworkAccessManager *m_nam;
};

#endif // OWNETCLOUDSERVER_H
