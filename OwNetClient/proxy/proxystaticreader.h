#ifndef PROXYSTATICREADER_H
#define PROXYSTATICREADER_H

#include <QObject>

class SocketHandler;
class ProxyRequest;

class ProxyStaticReader : public QObject
{
    Q_OBJECT
public:
    explicit ProxyStaticReader(SocketHandler *socketHandler, ProxyRequest *request, QObject *parent = 0);

    void read();

private:
    void error();

    SocketHandler *m_socketHandler;
    ProxyRequest *m_request;
};

#endif // PROXYSTATICREADER_H
