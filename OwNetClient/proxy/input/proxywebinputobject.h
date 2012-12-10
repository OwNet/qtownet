#ifndef PROXYWEBINPUTOBJECT_H
#define PROXYWEBINPUTOBJECT_H

#include <QNetworkReply>

#include "proxyinputobject.h"

class ProxyWebInputObject : public ProxyInputObject
{
    Q_OBJECT

public:
    ProxyWebInputObject(ProxyRequest *request, QObject *parent = 0);

protected:
    void readRequest();

private slots:
    void readReply();
    void error(QNetworkReply::NetworkError);
    void downloadFinished();
    void readResponseHeaders(QNetworkReply *reply);

private:
    void createReply();
    bool isClientOnline(int clientId) const;
    QString clientIp(int clientId) const;

    bool m_readHeaders;
    bool m_retryIfFailed;
    QList<int> m_clientsToTry;
};

#endif // PROXYWEBINPUTOBJECT_H
