#include "proxysocketrequest.h"

#include <QTcpSocket>
#include <QRegularExpression>
#include <QHostAddress>

ProxySocketRequest::ProxySocketRequest(QTcpSocket *socket, QObject *parent)
    : ProxyRequest(parent), m_socket(socket), m_socketIODevice(socket)
{
}

ProxySocketRequest::ProxySocketRequest(QIODevice *socketIODevice, QObject *parent)
    : ProxyRequest(parent), m_socket(NULL), m_socketIODevice(socketIODevice)
{
}

/**
 * @brief Reads request headers from socket.
 * @return Returns true if successful
 */
bool ProxySocketRequest::readFromSocket()
{
    for (int i = 0; m_socketIODevice->isOpen() && m_socketIODevice->canReadLine(); ++i) {
        QString readLine(m_socketIODevice->readLine());

        if (i == 0) {
            QStringList tuple = readLine.split(QRegularExpression("[ \r\n][ \r\n]*"));
            if (tuple.count() > 1) {
                QString requestMethod = tuple.first().toLower();
                if (requestMethod == "get")
                    setRequestType(GET);
                else if (requestMethod == "post")
                    setRequestType(POST);
                else if (requestMethod == "put")
                    setRequestType(PUT);
                else if (requestMethod == "delete")
                    setRequestType(DELETE);

                setUrl(QUrl::fromEncoded(tuple.at(1).toUtf8()));
            } else {
                return false;
            }
        } else {
            if (readLine == "\r\n") {
                setRequestBody(m_socketIODevice->readAll());
            } else {
                QStringList tokens = readLine.split(": ");
                if (tokens.count() < 2)
                    tokens = readLine.split(":");
                if (tokens.count() < 2)
                    continue;
                QString key = tokens.first();
                QString value = tokens.at(1);
                value.remove(QRegularExpression("[\r\n][\r\n]*"));

                addRequestHeader(key, value);
            }
        }
    }

    return true;
}

QIODevice *ProxySocketRequest::socket() const
{
    return m_socket;
}

QString ProxySocketRequest::peerAddress() const
{
    if (!m_socket)
        return "";
    return m_socket->peerAddress().toString();
}

quint16 ProxySocketRequest::peerPort() const
{
    if (!m_socket)
        return 0;
    return m_socket->peerPort();
}
