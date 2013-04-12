#include "proxywebinputobject.h"
#include "messagehelper.h"
#include "proxyrequest.h"
#include "proxydownloads.h"
#include "proxytrafficcounter.h"
#include "databaseselectquery.h"
#include "databasesettings.h"
#include "session.h"
#include "idatabaseselectquerywheregroup.h"
#include "requestreader.h"

#include <QNetworkReply>
#include <QNetworkProxy>
#include <QTcpSocket>
#include <QBuffer>

ProxyWebInputObject::ProxyWebInputObject(ProxyRequest *request, QObject *parent)
    : ProxyInputObject(request, parent),
      m_readHeaders(false),
      m_contentLength(0),
      m_responseLength(Unknown)
{
}

void ProxyWebInputObject::readRequest()
{
    createReply();

    ProxyDownloads::instance()->trafficCounter()->increaseCurrentTraffic();
}

void ProxyWebInputObject::socketConnectedToServer()
{
    m_socket->write(m_request->requestReader()->wholeRequestBody());
}

void ProxyWebInputObject::socketReadyRead()
{
    QByteArray *bytes = new QByteArray(m_socket->readAll());
    QBuffer *buffer = new QBuffer(bytes, this);
    buffer->open(QBuffer::ReadOnly);

    bool end = false;

    if (!m_readHeaders) {
        while (!buffer->atEnd()) {
            QByteArray line = buffer->readLine().trimmed();

            if (line.isEmpty()) {
                m_readHeaders = true;
                break;
            }
            if (m_responseLength != Unknown)
                continue;

            int colon = line.indexOf(':');
            if (colon > 0) {
                QString name = line.left(colon);
                if (name.compare("Content-length", Qt::CaseInsensitive) == 0) {
                    m_responseLength = Defined;
                    m_contentLength = line.mid(colon + 1).trimmed().toLong();
                } else if (name.compare("Transfer-encoding", Qt::CaseInsensitive) == 0) {
                    if (QString::fromUtf8(line.mid(colon + 1).trimmed()).compare("Chunked", Qt::CaseInsensitive) == 0)
                        m_responseLength == Chunked;
                }
            }
        }
        if (m_responseLength == Defined) {
            m_contentLength -= buffer->bytesAvailable();
            if (m_contentLength <= 0)
                end = true;
        } else {
            if (buffer->read(1).startsWith('0'))
                end = true;
        }

        buffer->seek(0);
    } else {
        if (m_responseLength == Defined) {
            m_contentLength -= bytes->length();
            if (m_contentLength <= 0)
                end = true;
        } else {
            if (bytes->startsWith('0'))
                end = true;
        }
    }

    emit readyRead(buffer);

    if (end)
        emit finished();
}

void ProxyWebInputObject::socketDisconnected()
{
    emit finished();
}

void ProxyWebInputObject::socketError(QAbstractSocket::SocketError error)
{
    MessageHelper::debug(QString("Download error: %1").arg(error));
    emit failed();
}

void ProxyWebInputObject::createReply()
{
    QString server = extractServer(m_request->url());

    m_socket = new QTcpSocket(this);

    connect(m_socket, SIGNAL(connected()), this, SLOT(socketConnectedToServer()));
    connect(m_socket, SIGNAL(readyRead()), this, SLOT(socketReadyRead()));
    connect(m_socket, SIGNAL(disconnected()), this, SLOT(socketDisconnected()));
    connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError(QAbstractSocket::SocketError)));

    if (!m_proxy.isEmpty()) {
        QStringList ipAndPort = clientIpAndPort(m_proxy).split(":");
        m_socket->setProxy(QNetworkProxy(QNetworkProxy::HttpProxy, ipAndPort.first(), ipAndPort.last().toInt()));
    }

    m_socket->connectToHost(getServerName(server), getPort(server));
}

bool ProxyWebInputObject::isClientOnline(const QString &clientId) const
{
    return Session().availableClients().contains(clientId);
}

QString ProxyWebInputObject::clientIpAndPort(const QString &clientId) const
{
    return Session().availableClients().value(clientId).toString();
}

QString ProxyWebInputObject::extractServer(const QString &fullUrl) const
{
    QString http = "http://";
    if (fullUrl.left(http.length()).compare(http, Qt::CaseInsensitive)==0)
    {
        int p = fullUrl.indexOf("/", http.length()+1);
        return fullUrl.mid(http.length(), p-http.length());
    }
    return fullUrl;
}

QString ProxyWebInputObject::getServerName(const QString &serverAndPort) const
{
    int p = serverAndPort.indexOf(':');
    if (p != -1)
        return serverAndPort.left(p);
    else
        return serverAndPort;
}

int ProxyWebInputObject::getPort(const QString &serverAndPort) const
{
    int p = serverAndPort.indexOf(':');
    if (p != -1)
    {
        QByteArray ba = serverAndPort.mid(p+1).toLatin1();
        return atoi(ba.data());
    }
    return 80;
}
