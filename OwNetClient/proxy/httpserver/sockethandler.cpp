#include "sockethandler.h"

#include "requestreader.h"
#include "proxyhandler.h"

#include <QMutexLocker>
#include <QTimer>

SocketHandler::SocketHandler(QTcpSocket * socketIn, QObject *parent) :
    QObject(parent),
    m_socketIn(socketIn),
    m_closed(false),
    m_requestReader(NULL),
    m_timeoutTimer(NULL),
    m_numProxyHandlers(0)
{
    qDebug() << "New socket";

    m_timeoutTimer = new QTimer(this);
    connect(m_timeoutTimer, SIGNAL(timeout()), this, SLOT(socketTimeout()));
    m_timeoutTimer->setInterval(Timeout);
}

bool SocketHandler::startConnection()
{
    if (!m_socketIn->isValid()) {
        qDebug() << "Input socket is invalid";
        return false;
    }
    m_socketIn->setParent(this);

    m_timeoutTimer->start();

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

    if (m_timeoutTimer->isActive())
        m_timeoutTimer->stop();
    m_timeoutTimer->start();

    if (!m_requestReader) {
        m_requestReader = new RequestReader(this, this);
    } else if (m_requestReader->status() == RequestReader::Complete) {
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

void SocketHandler::socketTimeout()
{
    if (m_timeoutTimer->isActive())
        m_timeoutTimer->stop();
    closeInput();
}

void SocketHandler::readingAborted()
{
    qDebug() << "Reading aborted";
    closeInput();
}

void SocketHandler::finishedReadingRequest()
{
    if (m_timeoutTimer->isActive())
        m_timeoutTimer->stop();

    qDebug() << m_requestReader->wholeRequestBody();

    m_numProxyHandlers++;
    ProxyHandler *handler = new ProxyHandler;
    handler->service(this);
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

void SocketHandler::proxyHandlerFinished(ProxyHandler *handler)
{
    m_numProxyHandlers--;
    handler->deleteLater();
    if (!m_socketIn) {
        if (m_numProxyHandlers <= 0)
            emit finished();
    } else {
        m_timeoutTimer->start();
    }
}

void SocketHandler::closeInput()
{
    if (m_socketIn) {
        if (m_socketIn->isOpen()) {
            m_socketIn->flush();
            m_socketIn->close();
        }
        m_socketIn = NULL;
    }


    if (m_numProxyHandlers <= 0)
        emit finished();
}
