#ifndef PROXYWEBREADER_H
#define PROXYWEBREADER_H

#include <QObject>

class SocketHandler;
class ProxyRequest;

class ProxyWebReader : public QObject
{
    Q_OBJECT
public:
    explicit ProxyWebReader(SocketHandler *socketHandler, ProxyRequest *request, QObject *parent = 0);

    void read();
    
signals:
    
public slots:
    
};

#endif // PROXYWEBREADER_H
