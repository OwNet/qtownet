#include "sockethandler.h"

#include "requestreader.h"
#include "proxyhandler.h"

#include <QMutexLocker>

SocketHandler::SocketHandler(QTcpSocket * socketIn, QObject *parent) :
    QObject(parent),
    m_socketIn(socketIn),
    m_socketOut(NULL),
    m_closed(false),
    m_requestReader(NULL)
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
    closeInput();
}

void SocketHandler::inputReadyRead()
{
    qDebug() << "Input ready read";

    if (!m_requestReader) {
        m_requestReader = new RequestReader(this, this);
    } else if (m_requestReader->status() == RequestReader::Complete) {
        if (m_socketOut) {
            m_socketOut->close();
            m_socketOut = NULL;
        }
        m_requestReader = new RequestReader(this, this);
    }

    auto status = m_requestReader->readFromSocket(m_socketIn);

    switch (status) {
    case RequestReader::Complete:
        finishedReadingRequest();
        break;
    case RequestReader::Abort:
        readingAborted();
        break;
    }
}

void SocketHandler::inputError(QAbstractSocket::SocketError error)
{
    qDebug() << "Input error: " << error;
    closeInput();
}

void SocketHandler::readingAborted()
{
    qDebug() << "Reading aborted";
    closeInput();
}

void SocketHandler::finishedReadingRequest()
{
    ProxyHandler *handler = new ProxyHandler;
    handler->service(this);
    return;

    if (m_socketOut) {
        m_socketOut->close();
    }

    QString server = extractServer(m_requestReader->url());

    m_socketOut = new QTcpSocket(this);

    connect(m_socketOut, SIGNAL(connected()), this, SLOT(outputConnectedToServer()));
    connect(m_socketOut, SIGNAL(readyRead()), this, SLOT(outputReadyRead()));
    connect(m_socketOut, SIGNAL(disconnected()), this, SLOT(outputDisconnected()));
    connect(m_socketOut, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(outputError(QAbstractSocket::SocketError)));

    m_socketOut->connectToHost(getServerName(server), getPort(server));
}

void SocketHandler::write(const QByteArray &data)
{
    if (m_socketIn)
        m_socketIn->write(data);
}

void SocketHandler::writeStatusCodeAndDescription(int statusCode, const QByteArray &description)
{
    QByteArray buffer;
    buffer.append("HTTP/1.1 ");
    buffer.append(QByteArray::number(statusCode));
    buffer.append(' ');
    buffer.append(description);
    buffer.append("\r\n");
    write(buffer);
}

void SocketHandler::writeHeaders(const QVariantMap &headers)
{
    QByteArray buffer;
    foreach(QString name, headers.keys()) {
        buffer.append(name);
        buffer.append(": ");
        buffer.append(headers.value(name).toByteArray());
        buffer.append("\r\n");
    }
    buffer.append("\r\n");
    write(buffer);
}

void SocketHandler::proxyHandlerFinished()
{
    m_socketIn->flush();
    //outputDisconnected();
}

void SocketHandler::outputConnectedToServer()
{
    qDebug() << "Output connected";
    m_socketOut->write(m_requestReader->wholeRequestBody());
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
    closeOutput();
}

void SocketHandler::outputError(QAbstractSocket::SocketError error)
{
    qDebug() << "Output error: " << error;
    closeOutput();
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
    m_socketIn->close();

    closeOutput();

    emit finished();
}

void SocketHandler::closeOutput()
{
    if (m_socketOut) {
        if (m_socketOut->isOpen()) {
            m_socketOut->disconnect(this);
            m_socketOut->close();
        }
        m_socketOut = NULL;
    }
}
