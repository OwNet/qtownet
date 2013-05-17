#ifndef PROXYWEBREADER_H
#define PROXYWEBREADER_H

#include <QObject>

class SocketHandler;
class ProxyRequest;
class QIODevice;
class ProxyHandlerSession;

class ProxyWebReader : public QObject
{
    Q_OBJECT
public:
    explicit ProxyWebReader(SocketHandler *socketHandler, ProxyRequest *request, ProxyHandlerSession *session, QObject *parent = 0);

    void read();

public slots:
    void readyRead();
    void finished();

signals:
    void iAmActive();

private:
    QIODevice *m_stream;
    ProxyRequest *m_request;
    SocketHandler *m_socketHandler;
    ProxyHandlerSession *m_session;
    int m_dependantObjectId;
};

#endif // PROXYWEBREADER_H
