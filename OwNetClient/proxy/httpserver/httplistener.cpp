/**
  @file
  @author Stefan Frings
*/

#include "httplistener.h"
#include "httpconnectionhandler.h"
#include <QCoreApplication>

HttpListener::HttpListener(QSettings* settings, QObject *parent)
    : QTcpServer(parent)
{
    Q_ASSERT(settings!=0);
    this->m_settings=settings;
    // Start listening
    int port=settings->value("listen_port").toInt();
    listen(QHostAddress::Any, port);
    if (!isListening()) {
        qCritical("HttpListener: Cannot bind on port %i: %s",port,qPrintable(errorString()));
    }
    else {
        qDebug("HttpListener: Listening on port %i",port);
    }
}

HttpListener::~HttpListener() {
    close();
    qDebug("HttpListener: closed");
}


void HttpListener::incomingConnection(qintptr socketDescriptor) {
#ifdef SUPERVERBOSE
    qDebug("HttpListener: New connection");
#endif
    HttpConnectionHandler* freeHandler = new HttpConnectionHandler(m_settings);

    freeHandler->setSocketDescriptor(socketDescriptor);
    // The descriptor is passed via signal/slot because the handler lives in another
    // thread and cannot open the socket when called by another thread.
    freeHandler->startHandling();
}
