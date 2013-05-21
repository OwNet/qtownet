#ifndef PROXYSERVICEREADER_H
#define PROXYSERVICEREADER_H

#include <QObject>

class SocketHandler;
class ProxyRequest;

class ServiceReader : public QObject
{
    Q_OBJECT
public:
    explicit ServiceReader(SocketHandler *socketHandler, ProxyRequest *request, QObject *parent = 0);

    void read();

private:
    SocketHandler *m_socketHandler;
    ProxyRequest *m_request;
};

#endif // PROXYSERVICEREADER_H
