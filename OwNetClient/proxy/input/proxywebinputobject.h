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

private:
    bool m_readHeaders;
};

#endif // PROXYWEBINPUTOBJECT_H
