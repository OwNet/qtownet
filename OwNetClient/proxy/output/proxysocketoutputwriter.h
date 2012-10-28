#ifndef PROXYSOCKETOUTPUTWRITER_H
#define PROXYSOCKETOUTPUTWRITER_H

class QTcpSocket;

#include "proxyoutputwriter.h"

class ProxySocketOutputWriter : public ProxyOutputWriter
{
    Q_OBJECT

public:
    ProxySocketOutputWriter(int socketDescriptor, QObject *parent = 0);

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
    QTcpSocket *m_socket;
};

#endif // PROXYSOCKETOUTPUTWRITER_H
