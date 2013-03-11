#ifndef WEBPINGER_H
#define WEBPINGER_H

#include <QObject>
#include <QStringList>

class QNetworkReply;
class IProxyConnection;

class WebPinger : public QObject
{
    Q_OBJECT
public:
    explicit WebPinger(IProxyConnection *proxyConnection, QObject *parent = 0);

    void ping();

private slots:
    void replyFinished(QNetworkReply *reply);

private:
    QString nextUrl();

    IProxyConnection *m_proxyConnection;
    QStringList m_urlsToPing;
    int m_nextUrlIndex;
    bool m_lastOK;
};

#endif // WEBPINGER_H
