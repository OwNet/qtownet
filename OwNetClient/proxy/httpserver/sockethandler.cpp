#include "sockethandler.h"

#include <QMutex>
#include <QMutexLocker>

SocketHandler::SocketHandler(QTcpSocket * socketIn, QObject *parent) :
    QObject(parent),
    m_socketIn(socketIn),
    m_socketOut(NULL),
    m_isOutputConnected(false),
    m_closed(false)
{
    qDebug() << "New socket";
}

bool SocketHandler::startConnection()
{
    if (!m_socketIn->isValid()) {
        qDebug() << "Input socket is invalid";
        return false;
    }
    m_socketIn->setParent(this);

    connect(m_socketIn, SIGNAL(disconnected()), this, SLOT(inputDisconnected()));
    connect(m_socketIn, SIGNAL(readyRead()), this, SLOT(inputReadyRead()));
    connect(m_socketIn, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(inputError(QAbstractSocket::SocketError)));

    return true;
}

void SocketHandler::start()
{
    if (!startConnection())
        closeInput();
}

void SocketHandler::inputDisconnected()
{
    qDebug() << "Input disconnected";
    if (m_socketOut && m_socketOut->isValid()) {
        m_socketOut->disconnect();
        m_socketOut->disconnectFromHost();
    }
    closeInput();
}

void SocketHandler::inputReadyRead()
{
    qDebug() << "Input ready read";

    static QMutex lock;
    QMutexLocker locker(&lock);

    QByteArray bytes = m_socketIn->readAll();
    m_requestBytes.append(bytes);
    int i = bytes.indexOf(" ");
    if (i < 5)
        m_isOutputConnected = false;

    if (!m_isOutputConnected) {
        m_isOutputConnected = true;
        if (i < 0)
            return;

        qDebug() << bytes.mid(0, i);
        int n = bytes.indexOf(" ", i + 1) - (i + 1);
        QString url = QString(bytes.mid(i + 1, n));
        QString server = extractServer(url);
        qDebug() << server;

        if (m_socketOut) {
            m_socketOut->close();
        }

        m_socketOut = new QTcpSocket(this);

        connect(m_socketOut, SIGNAL(connected()), this, SLOT(outputConnectedToServer()));
        connect(m_socketOut, SIGNAL(readyRead()), this, SLOT(outputReadyRead()));
        connect(m_socketOut, SIGNAL(disconnected()), this, SLOT(outputDisconnected()));
        connect(m_socketOut, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(outputError(QAbstractSocket::SocketError)));

        m_socketOut->connectToHost(getServerName(server), getPort(server));
    }
}

void SocketHandler::inputError(QAbstractSocket::SocketError error)
{
    qDebug() << "Input error: " << error;
    closeInput();
}

void SocketHandler::outputConnectedToServer()
{
    qDebug() << "Output connected";
    m_socketOut->write(m_requestBytes);
}

void SocketHandler::outputReadyRead()
{
    qDebug() << "Output ready read";
    QByteArray data = m_socketOut->readAll();
    m_socketIn->write(data);
    m_socketIn->flush();
}

void SocketHandler::outputDisconnected()
{
    qDebug() << "Output disconnected";
    closeInput();
}

void SocketHandler::outputError(QAbstractSocket::SocketError error)
{
    qDebug() << "Output error: " << error;
    closeInput();
}

QString SocketHandler::extractServer(const QString &fullUrl) const
{
    QString http = "http://";
    if (fullUrl.left(http.length()).compare(http, Qt::CaseInsensitive)==0)
    {
        int p = fullUrl.indexOf("/", http.length()+1);
        return fullUrl.mid(http.length(), p-http.length());
    }
    return fullUrl;
}

QString SocketHandler::getServerName(const QString &serverAndPort) const
{
    int p = serverAndPort.indexOf(':');
    if (p != -1)
        return serverAndPort.left(p);
    else
        return serverAndPort;
}

int SocketHandler::getPort(const QString &serverAndPort) const
{
    int p = serverAndPort.indexOf(':');
    if (p != -1)
    {
        QByteArray ba = serverAndPort.mid(p+1).toLatin1();
        return atoi(ba.data());
    }
    return 80;
}

void SocketHandler::closeInput()
{
    if (m_closed)
        return;
    m_closed = true;

    if (m_socketIn->isOpen()) {
        m_socketIn->flush();
    }
    m_socketIn->disconnect();
    m_socketIn->disconnectFromHost();

    if (m_socketOut && m_socketOut->isOpen()) {
        m_socketOut->close();
    }

    closeOutput();
    m_isOutputConnected = false;

    emit finished();
}

void SocketHandler::closeOutput()
{
    if (m_socketOut)
        m_socketOut = NULL;
}
