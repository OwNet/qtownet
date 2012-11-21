#include "proxysocketoutputwriter.h"

#include "proxydownload.h"
#include "proxyrequest.h"
#include "messagehelper.h"
#include "proxyinputobject.h"
#include "proxyhandlersession.h"

#include <QTcpSocket>
#include <QFile>
#include <QSemaphore>

ProxySocketOutputWriter::ProxySocketOutputWriter(int socketDescriptor, ProxyHandlerSession *proxyHandlerSession)
    : ProxyOutputWriter(proxyHandlerSession), m_socketDescriptor(socketDescriptor), m_writtenToSocket(false), m_foundBody(false), m_socket(NULL)
{
}

/**
 * @brief Opens the socket and starts the download, triggered from outside.
 */
void ProxySocketOutputWriter::startDownload()
{
    m_socket = new QTcpSocket(this);
    connect(m_socket, SIGNAL(readyRead()), this, SLOT(readRequest()));
    m_socket->setSocketDescriptor(m_socketDescriptor);
}

/**
 * @brief Triggered when socket is ready to be read.
 */
void ProxySocketOutputWriter::readRequest()
{
    ProxyRequest *request = new ProxyRequest(m_socket, m_proxyHandlerSession);

    if (!request->readFromSocket()) {
        MessageHelper::debug("Failed to read from socket.");
        forceQuit();
        return;
    }
    MessageHelper::debug(request->url());

    createDownload(request);
}

/**
 * @brief Close socket and end download.
 */
void ProxySocketOutputWriter::virtualClose()
{
    if (m_socket) {
        if (m_socket->isOpen()) {
            m_socket->flush();
            m_socket->disconnectFromHost();
            if (m_socket->state() != QAbstractSocket::UnconnectedState)
                m_socket->waitForDisconnected();
        }
    }
}

/**
 * @brief Read data from a stream and write to socket.
 * @param ioDevice Stream of bytes
 */
void ProxySocketOutputWriter::read(QIODevice *ioDevice)
{
    if (!m_socket || !m_socket->isOpen() || !ioDevice)
        return;

    if (!m_writtenToSocket) {
        m_writtenToSocket = true;

        QTextStream os(m_socket);
        os.setAutoDetectUnicode(false);

        os << "HTTP/1.0 "
           << m_proxyDownload->inputObject()->httpStatusCode()
           << " "
           << m_proxyDownload->inputObject()->httpStatusDescription()
           << "\r\n";
        foreach (QString key, m_proxyDownload->inputObject()->responseHeaders().keys()) {
            os << key
               << ": "
               << m_proxyDownload->inputObject()->responseHeaders().value(key).toString()
               << "\r\n";
        }
        os << "\r\n";
        os.flush();
    }

    if (!m_foundBody && m_proxyDownload->inputObject()->contentType().toLower().contains("text/html")) {
        while (!ioDevice->atEnd()) {
            QByteArray lineBytes = ioDevice->readLine();
            QString line = QString::fromLatin1(lineBytes);
            if (line.contains("<body")) {
                //m_socket->write(QString("<script type=\"text/javascript\" src=\"http://ownet.tym.sk/script.js\"></script>")
                //                .toLatin1());
                m_foundBody = true;
            }
            m_socket->write(lineBytes);
        }
    }
    else {
        m_foundBody = true;
        m_socket->write(ioDevice->readAll());
    }
}
