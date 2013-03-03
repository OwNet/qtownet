#ifndef OWNETCLIENT_H
#define OWNETCLIENT_H

#include <QObject>
#include <QString>
#include <QProcess>
#include <QDateTime>

class OwNetCloudServer;

class OwNetClient : public QObject
{
    Q_OBJECT
public:
    explicit OwNetClient(OwNetCloudServer *, QObject *parent = 0);
    void start();
    
signals:
    
public slots:
    void finished();
    void readyRead();
private:
    QProcess *m_process;
    QString m_path;
    QString m_output;
    QDateTime m_lastError;
    OwNetCloudServer *m_cloudServer;
};

#endif // OWNETCLIENT_H
