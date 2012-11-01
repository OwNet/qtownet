#ifndef PROXYSOCKETOUTPUTWRITER_H
#define PROXYSOCKETOUTPUTWRITER_H

class QTcpSocket;
class QSemaphore;

#include "proxyoutputwriter.h"

class ProxySocketOutputWriter : public ProxyOutputWriter
{
    Q_OBJECT

public:
    ProxySocketOutputWriter(int socketDescriptor, QObject *parent = 0);
    ~ProxySocketOutputWriter();

    enum {
        BufferSize = 8192
    };

    void startDownload();
    void finish();

private slots:
    void readRequest();

signals:
    void finished();

protected:
    void close();
    void read(QIODevice *ioDevice);
    void readReply(QIODevice *ioDevice, ProxyInputObject *inputObject);

private:
    int m_socketDescriptor;
    bool m_writtenToSocket;
    bool m_foundBody;
    QTcpSocket *m_socket;
    QSemaphore *m_outputSemaphore;
};

#endif // PROXYSOCKETOUTPUTWRITER_H
