#ifndef PROXYWEBINPUTOBJECT_H
#define PROXYWEBINPUTOBJECT_H

#include <QAbstractSocket>
#include <QStringList>

#include "proxyinputobject.h"

class QTcpSocket;
class QTimer;

class ProxyWebInputObject : public ProxyInputObject
{
    Q_OBJECT

public:
    enum {
        Timeout = 10000
    };
    enum ResponseLength {
        Chunked,
        Defined,
        Unknown
    };

    ProxyWebInputObject(ProxyRequest *request, QObject *parent = 0);

    InputType inputType() const { return Web; }
    void setProxy(const QString &proxy) { m_proxy = proxy; }

    bool headersInBody() const { return true; }

protected:
    void readRequest();

private slots:
    void socketConnectedToServer();
    void socketReadyRead();
    void socketDisconnected();
    void socketError(QAbstractSocket::SocketError error);
    void responseTimeout();

private:
    void createReply();
    bool isClientOnline(const QString &clientId) const;
    QString clientIpAndPort(const QString &clientId) const;
    QString extractServer(const QString &fullUrl) const;
    QString serverName(const QString &serverAndPort) const;
    int port(const QString &serverAndPort) const;

    bool m_readHeaders;
    QStringList m_clientsToTry;
    QString m_proxy;
    QTcpSocket *m_socket;
    long m_contentLength;
    ResponseLength m_responseLength;
    QTimer *m_timeoutTimer;
};

#endif // PROXYWEBINPUTOBJECT_H
