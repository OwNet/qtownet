#ifndef WEBSOCKET_H
#define WEBSOCKET_H

#include <QObject>
#include <QTcpSocket>
#include <QStringList>

class ProxyRequest;
class QTimer;

class WebSocket : public QObject
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

    explicit WebSocket(ProxyRequest *request, QIODevice *output, QObject *parent = 0);

    void readRequest();
    void setProxy(const QString &proxy) { m_proxy = proxy; }

signals:
    void readyRead();
    void finished();
    void failed();

private slots:
    void socketConnectedToServer();
    void socketReadyRead();
    void socketDisconnected();
    void socketError(QAbstractSocket::SocketError error);
    void responseTimeout();

private:
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
    ProxyRequest *m_request;
    QIODevice *m_output;
};

#endif // WEBSOCKET_H
