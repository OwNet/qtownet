#ifndef PROXYWEBINPUTOBJECT_H
#define PROXYWEBINPUTOBJECT_H

#include <QNetworkReply>
#include <QStringList>

#include "proxyinputobject.h"

class ProxyWebInputObject : public ProxyInputObject
{
    Q_OBJECT

public:
    ProxyWebInputObject(ProxyRequest *request, QObject *parent = 0);

    InputType inputType() const { return Web; }
    void setProxy(const QString &proxy) { m_proxy = proxy; }

protected:
    void readRequest();

private slots:
    void readReply();
    void error(QNetworkReply::NetworkError);
    void downloadFinished();
    void readResponseHeaders(QNetworkReply *reply);

private:
    void createReply();
    bool isClientOnline(const QString &clientId) const;
    QString clientIpAndPort(const QString &clientId) const;

    bool m_readHeaders;
    QStringList m_clientsToTry;
    QString m_proxy;
};

#endif // PROXYWEBINPUTOBJECT_H
