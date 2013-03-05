#ifndef PROXYWEBINPUTOBJECT_H
#define PROXYWEBINPUTOBJECT_H

#include <QNetworkReply>

#include "proxyinputobject.h"

class ProxyWebInputObject : public ProxyInputObject
{
    Q_OBJECT

public:
    ProxyWebInputObject(ProxyRequest *request, QObject *parent = 0);

    InputType inputType() const { return Web; }

protected:
    void readRequest();

private slots:
    void readReply();
    void error(QNetworkReply::NetworkError);
    void downloadFinished();
    void readResponseHeaders(QNetworkReply *reply);

private:
    void createReply();
    bool isClientOnline(uint clientId) const;
    QString clientIpAndPort(uint clientId) const;

    bool m_readHeaders;
    bool m_retryIfFailed;
    QList<uint> m_clientsToTry;
};

#endif // PROXYWEBINPUTOBJECT_H
