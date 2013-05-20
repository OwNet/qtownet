#ifndef PROXYSTATICREADER_H
#define PROXYSTATICREADER_H

#include <QObject>

class SocketHandler;
class ProxyRequest;

class StaticReader : public QObject
{
    Q_OBJECT
public:
    explicit StaticReader(SocketHandler *socketHandler, ProxyRequest *request, QObject *parent = 0);

    void read();

private:
    void error();

    SocketHandler *m_socketHandler;
    ProxyRequest *m_request;
};

#endif // PROXYSTATICREADER_H
