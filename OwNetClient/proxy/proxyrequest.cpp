#include "proxyrequest.h"
#include "messagehelper.h"

#include <QNetworkRequest>
#include <QStringList>
#include <QTcpSocket>

ProxyRequest::ProxyRequest(QTcpSocket *socket)
    : m_socket(socket)
{}

bool ProxyRequest::readFromSocket()
{
    for (int i = 0; m_socket->isOpen() && m_socket->canReadLine(); ++i) {
        QString readLine(m_socket->readLine());

        if (i == 0) {
            QStringList tuple = readLine.split(QRegExp("[ \r\n][ \r\n]*"));
            if (tuple.count() > 1) {
                m_requestMethod = tuple.first().toLower();
                m_networkRequest.setUrl(QUrl(tuple.at(1)));
            } else {
                return false;
            }
        } else {
            QStringList tokens = readLine.split(":");
            if (tokens.count() < 2)
                continue;
            QString key = tokens.first();
            QString value = tokens.at(1);
            value.remove(QRegExp("[\r\n][\r\n]*"));

            m_networkRequest.setRawHeader(key.toLatin1(), value.toLatin1());
        }
    }
    return true;
}

ProxyRequest::RequestType ProxyRequest::requestType()
{
    if (m_requestMethod == "get")
        return GET;
    else if (m_requestMethod == "post")
        return POST;
    else if (m_requestMethod == "put")
        return PUT;
    else if (m_requestMethod == "delete")
        return DELETE;
    return UNKNOWN;
}
