#include "testserver.h"

#include "messagehelper.h"
#include "proxydownloads.h"

#include <QThread>
#include <QTcpSocket>
#include <QDebug>

TestServer::TestServer(QObject *parent)
    : QTcpServer(parent)
{
    ProxyDownloads::instance()->setApplicationProxy("localhost", Port);
}

void TestServer::readFromSocket()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    while (!socket->atEnd()) {
        qDebug() << socket->readAll();
    }

    QTextStream os(socket);
    os.setAutoDetectUnicode(false);

    os << "HTTP/1.0 200 OK \r\n";
    os << "Content-type: text/html\r\n";
    os << "\r\n";
    os.flush();
    os << "Hello";
    socket->close();
}

/**
 * @brief Received new open socket with request.
 * @param handle Socket descriptor
 */
void TestServer::incomingConnection(qintptr handle)
{
    QTcpSocket *socket = new QTcpSocket(this);
    socket->setSocketDescriptor(handle);

    connect(socket, SIGNAL(readyRead()), this, SLOT(readFromSocket()));
}
