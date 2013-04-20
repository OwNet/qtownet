#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <QObject>

class QTcpServer;

class HttpServer : public QObject
{
    Q_OBJECT
public:
    explicit HttpServer(QObject *parent = 0);

public slots:
    void newConnection();

private:
    QTcpServer *m_server;
};

#endif // HTTPSERVER_H
