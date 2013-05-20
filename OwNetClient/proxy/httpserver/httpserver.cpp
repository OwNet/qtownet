#include "httpserver.h"
#include "settings.h"
#include "messagehelper.h"

#include <QTcpServer>
#include <QThread>

#include "sockethandler.h"

HttpServer::HttpServer(QObject *parent) :
    QObject(parent)
{
    Settings settings;

    int port = settings.listenPort();

    m_server = new QTcpServer(this);
    connect(m_server, SIGNAL(newConnection()), SLOT(newConnection()));
    bool ok = m_server->listen(QHostAddress::Any, port);
    if (ok)
        MessageHelper::debug(QObject::tr("Listening on port %1").arg(port));
    else
        MessageHelper::debug(QObject::tr("Failed to listen on port %1").arg(port));
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
