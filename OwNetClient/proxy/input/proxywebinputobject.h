#ifndef PROXYWEBINPUTOBJECT_H
#define PROXYWEBINPUTOBJECT_H

#include <QNetworkReply>

#include "proxyinputobject.h"

class ProxyWebInputObject : public ProxyInputObject
{
    Q_OBJECT

public:
    ProxyWebInputObject(ProxyRequest *request, QObject *parent = 0);

    void readRequest();

    const QString httpStatusCode() { return m_httpStatusCode; }
    const QString httpStatusDescription() { return m_httpStatusDescription; }

private slots:
    void readReply();
    void error(QNetworkReply::NetworkError);
    void downloadFinished();

private:
    bool m_readHeaders;
    QString m_httpStatusCode;
    QString m_httpStatusDescription;
};

#endif // PROXYWEBINPUTOBJECT_H
