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
    bool m_retryIfFailed;
    QStringList m_clientsToTry;
};

#endif // PROXYWEBINPUTOBJECT_H
