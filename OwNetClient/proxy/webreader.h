#ifndef PROXYWEBREADER_H
#define PROXYWEBREADER_H

#include <QObject>
#include <QMutex>

class SocketHandler;
class ProxyRequest;
class QIODevice;
class ProxyHandlerSession;

class WebReader : public QObject
{
    Q_OBJECT
public:
    explicit WebReader(SocketHandler *socketHandler, ProxyRequest *request, ProxyHandlerSession *session, QObject *parent = 0);

    void read();

public slots:
    void readyRead();
    void finished();
    void failed();

signals:
    void iAmActive();

private:
    QIODevice *m_stream;
    ProxyRequest *m_request;
    SocketHandler *m_socketHandler;
    ProxyHandlerSession *m_session;
    int m_dependentObjectId;
    bool m_writtenToSocket;
    QMutex m_readMutex;
};

#endif // PROXYWEBREADER_H
