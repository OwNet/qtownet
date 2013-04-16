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
#include <QTimer>
#include <QBuffer>

ProxyWebInputObject::ProxyWebInputObject(ProxyRequest *request, QObject *parent)
    : ProxyInputObject(request, parent),
      m_readHeaders(false),
      m_contentLength(0),
      m_responseLength(Unknown),
      m_timeoutTimer(NULL)
{
}

void ProxyWebInputObject::readRequest()
{
    createReply();

    ProxyDownloads::instance()->trafficCounter()->increaseCurrentTraffic();
}

void ProxyWebInputObject::socketConnectedToServer()
{
    QByteArray bytes = m_request->requestReader()->wholeRequestBody();
    if (m_proxy.isEmpty()) {
        int i = bytes.indexOf(" ");
        if (i < 0)
            return;

        int n = bytes.indexOf(" ", i + 1) - (i + 1);
        QString url = QString(bytes.mid(i + 1, n));
        url.remove(0, url.indexOf('/', 7));

        m_socket->write(bytes.mid(0, i + 1));
        m_socket->write(url.toUtf8());
        m_socket->write(bytes.mid(n + i + 1));
    } else {
        m_socket->write(bytes);
    }
}

void ProxyWebInputObject::socketReadyRead()
{
    if (m_timeoutTimer) {
        if (m_timeoutTimer->isActive())
            m_timeoutTimer->stop();
        m_timeoutTimer->start(Timeout);
    }

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
                        m_responseLength = Chunked;
                }
            }
        }
        if (m_responseLength == Defined) {
            m_contentLength -= buffer->bytesAvailable();
            if (m_contentLength <= 0)
                end = true;
        } else if (m_responseLength == Chunked) {
            if (bytes->endsWith("0\r\n\r\n"))
                end = true;
        }

        buffer->seek(0);
    } else {
        if (m_responseLength == Defined) {
            m_contentLength -= bytes->length();
            if (m_contentLength <= 0)
                end = true;
        } else if (m_responseLength == Chunked) {
            if (bytes->endsWith("0\r\n\r\n"))
                end = true;
        }
    }

    emit readyRead(buffer);

    if (end)
        emit finished();
}

void ProxyWebInputObject::socketDisconnected()
{
    if (m_timeoutTimer && m_timeoutTimer->isActive())
        m_timeoutTimer->stop();
    emit finished();
}

void ProxyWebInputObject::socketError(QAbstractSocket::SocketError error)
{
    if (m_timeoutTimer && m_timeoutTimer->isActive())
        m_timeoutTimer->stop();
    MessageHelper::debug(QString("Download error: %1").arg(error));
    emit failed();
}

void ProxyWebInputObject::responseTimeout()
{
    m_socket->close();
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
        QStringList ipAndPort = m_proxy.split(":");
        m_socket->setProxy(QNetworkProxy(QNetworkProxy::DefaultProxy, ipAndPort.first(), ipAndPort.last().toInt()));
    }

    m_socket->connectToHost(serverName(server), port(server));

    m_timeoutTimer = new QTimer(this);
    connect(m_timeoutTimer, SIGNAL(timeout()), this, SLOT(responseTimeout()));
    m_timeoutTimer->start(Timeout);
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

QString ProxyWebInputObject::serverName(const QString &serverAndPort) const
{
    int p = serverAndPort.indexOf(':');
    if (p != -1)
        return serverAndPort.left(p);
    else
        return serverAndPort;
}

int ProxyWebInputObject::port(const QString &serverAndPort) const
{
    int p = serverAndPort.indexOf(':');
    if (p != -1)
    {
        QByteArray ba = serverAndPort.mid(p+1).toLatin1();
        return atoi(ba.data());
    }
    return 80;
}
