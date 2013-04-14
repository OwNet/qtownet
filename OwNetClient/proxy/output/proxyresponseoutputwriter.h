#ifndef PROXYRESPONSEOUTPUTWRITER_H
#define PROXYRESPONSEOUTPUTWRITER_H

class QTcpSocket;
class QSemaphore;
class SocketHandler;

#include "proxyoutputwriter.h"

#include <QMap>

/**
 * @brief Reads data from ProxyDownload and outputs them to socket.
 */
class ProxyResponseOutputWriter : public ProxyOutputWriter
{
    Q_OBJECT

public:
    ProxyResponseOutputWriter(SocketHandler *socketHandler, ProxyHandlerSession *proxyHandler);

    enum {
        BufferSize = 8192
    };

    void startDownload();
    static QList<QString> dumpOpenRequests();

protected:
    void virtualClose();
    void read(QIODevice *ioDevice);
    void readReply(QIODevice *ioDevice, ProxyInputObject *inputObject);
    void error() {}

private:
    SocketHandler *m_socketHandler;
    uint m_requestHashCode;
    bool m_hasWrittenResponseHeaders;
    bool m_foundBody;

    static QMap<int, QString> *m_openRequests;
};

#endif // PROXYRESPONSEOUTPUTWRITER_H
