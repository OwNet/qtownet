#include "httpserver.h"

#include <QTcpServer>
#include <QThread>

#include "sockethandler.h"

HttpServer::HttpServer(QObject *parent) :
    QObject(parent)
{
    m_server = new QTcpServer(this);
    connect(m_server, SIGNAL(newConnection()), SLOT(newConnection()));
    bool ok = m_server->listen(QHostAddress::Any, 8081);
    qDebug() << ok;
}

void HttpServer::newConnection()
{
    int connections = 0;
    while (m_server->hasPendingConnections())
    {
        QTcpSocket *socket = m_server->nextPendingConnection();

        QThread *thread = new QThread;
        socket->setParent(NULL);
        socket->moveToThread(thread);
        SocketHandler *handler = new SocketHandler(socket);
        handler->moveToThread(thread);
        connect(thread, SIGNAL(started()), handler, SLOT(start()), Qt::QueuedConnection);
        connect(handler, SIGNAL(finished()), thread, SLOT(quit()));
        connect(handler, SIGNAL(finished()), handler, SLOT(deleteLater()));
        connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
        thread->start();
        connections++;
    }
}
