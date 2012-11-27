#ifndef PROXYSOCKETOUTPUTWRITER_H
#define PROXYSOCKETOUTPUTWRITER_H

class QTcpSocket;
class QSemaphore;

#include "proxyoutputwriter.h"

#include <QMap>

/**
 * @brief Reads data from ProxyDownload and outputs them to socket.
 */
class ProxySocketOutputWriter : public ProxyOutputWriter
{
    Q_OBJECT

public:
    ProxySocketOutputWriter(int socketDescriptor, ProxyHandlerSession *proxyHandler);

    enum {
        BufferSize = 8192
    };

    void startDownload();
    static QList<QString> dumpOpenRequests();

private slots:
    void readRequest();

protected:
    void virtualClose();
    void read(QIODevice *ioDevice);
    void readReply(QIODevice *ioDevice, ProxyInputObject *inputObject);
    void error() {}

private:
    int m_socketDescriptor;
    int m_requestHashCode;
    bool m_writtenToSocket;
    bool m_foundBody;
    QTcpSocket *m_socket;

    static QMap<int, QString> *m_openRequests;
};

#endif // PROXYSOCKETOUTPUTWRITER_H
