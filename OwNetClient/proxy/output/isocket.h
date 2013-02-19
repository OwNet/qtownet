#ifndef ISOCKET_H
#define ISOCKET_H

#include <QAbstractSocket>

class QIODevice;

class ISocket
{
public:
    virtual void setSocketDescriptor(int socketDescriptor) = 0;
    virtual QIODevice *ioDevice() = 0;
    virtual void disconnectFromHost() {}
    virtual bool waitForDisconnected(int msecs = 3000) { return true; }
    virtual bool flush() { return true; }
    virtual QAbstractSocket::SocketState state() { return QAbstractSocket::UnconnectedState; }
};

#endif // ISOCKET_H
