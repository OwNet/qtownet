#ifndef PROXYSOCKETOUTPUTWRITER_H
#define PROXYSOCKETOUTPUTWRITER_H

class QTcpSocket;
class QSemaphore;

#include "proxyoutputwriter.h"

class ProxySocketOutputWriter : public ProxyOutputWriter
{
    Q_OBJECT

public:
    ProxySocketOutputWriter(int socketDescriptor, ProxyHandler *proxyHandler);

    enum {
        BufferSize = 8192
    };

    void startDownload();

private slots:
    void readRequest();

protected:
    void virtualClose();
    void read(QIODevice *ioDevice);
    void readReply(QIODevice *ioDevice, ProxyInputObject *inputObject);
    void error() {}

private:
    int m_socketDescriptor;
    bool m_writtenToSocket;
    bool m_foundBody;
    QTcpSocket *m_socket;
};

#endif // PROXYSOCKETOUTPUTWRITER_H
