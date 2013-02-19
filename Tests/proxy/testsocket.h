#ifndef TESTSOCKET_H
#define TESTSOCKET_H

#include <QObject>

#include "isocket.h"

class TestSocket : public QObject, public ISocket
{
    Q_OBJECT
public:
    explicit TestSocket(QByteArray *data, QObject *parent = 0);

    void setSocketDescriptor(int socketDescriptor) {}
    QIODevice *ioDevice();

private:
    QByteArray *m_data;
};

#endif // TESTSOCKET_H
