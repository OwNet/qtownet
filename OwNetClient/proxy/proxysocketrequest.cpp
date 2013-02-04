#include "proxysocketrequest.h"

#include <QRegularExpression>

ProxySocketRequest::ProxySocketRequest(QIODevice *socket, QObject *parent)
    : ProxyRequest(parent), m_socket(socket)
{
}

/**
 * @brief Reads request headers from socket.
 * @return Returns true if successful
 */
bool ProxySocketRequest::readFromSocket()
{
    for (int i = 0; m_socket->isOpen() && m_socket->canReadLine(); ++i) {
        QString readLine(m_socket->readLine());

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
                setRequestBody(m_socket->readAll());
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
