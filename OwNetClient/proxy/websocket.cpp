#include "websocket.h"

#include "proxyrequest.h"
#include "proxydownloads.h"
#include "proxytrafficcounter.h"
#include "requestreader.h"
#include "session.h"
#include "iwebdownload.h"
#include "websocketoutput.h"

#include <QBuffer>
#include <QTimer>

WebSocket::WebSocket(ProxyRequest *request, IWebDownload *webDownload, WebSocketOutput *output, QObject *parent)
    : QObject(parent),
      m_readHeaders(false),
      m_contentLength(0),
      m_responseLength(Unknown),
      m_timeoutTimer(NULL),
      m_request(request),
      m_outputFile(output),
      m_sizeWritten(0),
      m_webDownload(webDownload)
{}

void WebSocket::readRequest()
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

    m_timeoutTimer = new QTimer(this);
    connect(m_timeoutTimer, SIGNAL(timeout()), this, SLOT(responseTimeout()));
    m_timeoutTimer->start(Timeout);

    m_socket->connectToHost(serverName(server), port(server));

    ProxyDownloads::instance()->trafficCounter()->increaseCurrentTraffic();
}

void WebSocket::socketConnectedToServer()
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

void WebSocket::socketReadyRead()
{
    if (m_timeoutTimer) {
        if (m_timeoutTimer->isActive())
            m_timeoutTimer->stop();
        m_timeoutTimer->start(Timeout);
    }

    QByteArray bytes = m_socket->readAll();
    QBuffer buffer(&bytes, this);
    buffer.open(QBuffer::ReadOnly);

    bool end = false;

    if (!m_readHeaders) {
        while (!buffer.atEnd()) {
            QByteArray line = buffer.readLine().trimmed();

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
            m_contentLength -= buffer.bytesAvailable();
            if (m_contentLength <= 0)
                end = true;
        } else if (m_responseLength == Chunked) {
            if (bytes.endsWith("0\r\n\r\n"))
                end = true;
        }
    } else {
        if (m_responseLength == Defined) {
            m_contentLength -= bytes.length();
            if (m_contentLength <= 0)
                end = true;
        } else if (m_responseLength == Chunked) {
            if (bytes.endsWith("0\r\n\r\n"))
                end = true;
        }
    }

    m_sizeWritten += bytes.size();
    m_outputFile->write(bytes);
    m_outputFile->flush();
    emit readyRead();

    if (end)
        finished(m_sizeWritten);
}

void WebSocket::socketDisconnected()
{
    if (m_timeoutTimer && m_timeoutTimer->isActive())
        m_timeoutTimer->stop();

    finished(m_sizeWritten);
}

void WebSocket::socketError(QAbstractSocket::SocketError error)
{
    if (m_timeoutTimer && m_timeoutTimer->isActive())
        m_timeoutTimer->stop();

    failed();
}

void WebSocket::responseTimeout()
{
    if (m_timeoutTimer && m_timeoutTimer->isActive())
        m_timeoutTimer->stop();

    m_socket->close();
    failed();
}

bool WebSocket::isClientOnline(const QString &clientId) const
{
    return Session().availableClients().contains(clientId);
}

QString WebSocket::clientIpAndPort(const QString &clientId) const
{
    return Session().availableClients().value(clientId).toString();
}

QString WebSocket::extractServer(const QString &fullUrl) const
{
    QString http = "http://";
    if (fullUrl.left(http.length()).compare(http, Qt::CaseInsensitive)==0)
    {
        int p = fullUrl.indexOf("/", http.length()+1);
        return fullUrl.mid(http.length(), p-http.length());
    }
    return fullUrl;
}

QString WebSocket::serverName(const QString &serverAndPort) const
{
    int p = serverAndPort.indexOf(':');
    if (p != -1)
        return serverAndPort.left(p);
    else
        return serverAndPort;
}

int WebSocket::port(const QString &serverAndPort) const
{
    int p = serverAndPort.indexOf(':');
    if (p != -1)
    {
        QByteArray ba = serverAndPort.mid(p+1).toLatin1();
        return atoi(ba.data());
    }
    return 80;
}

void WebSocket::finished(qint64 size)
{
    if (m_outputFile) {
        m_outputFile->close();
        m_outputFile->deleteLater();
        m_outputFile = NULL;
    }
    m_webDownload->downloadFinished(size);
    deleteLater();
}

void WebSocket::failed()
{
    if (m_outputFile) {
        m_outputFile->remove();
        m_outputFile->deleteLater();
        m_outputFile = NULL;
    }
    m_webDownload->downloadFailed();
    deleteLater();
}
