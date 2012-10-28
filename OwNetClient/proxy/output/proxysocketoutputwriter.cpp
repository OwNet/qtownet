#include "proxysocketoutputwriter.h"

#include "proxydownload.h"
#include "proxyrequest.h"
#include "messagehelper.h"
#include "proxyinputobject.h"

#include <QTcpSocket>

ProxySocketOutputWriter::ProxySocketOutputWriter(int socketDescriptor, QObject *parent) :
    ProxyOutputWriter(parent), m_socketDescriptor(socketDescriptor), m_writtenToSocket(false), m_socket(NULL)
{
}

/**
 * @brief Opens the socket and starts the download, triggered from outside.
 */
void ProxySocketOutputWriter::startDownload()
{
    m_socket = new QTcpSocket();
    connect(m_socket, SIGNAL(readyRead()), this, SLOT(readRequest()));
    m_socket->setSocketDescriptor(m_socketDescriptor);
}

/**
 * @brief Force finish downloading request, triggered from outside.
 */
void ProxySocketOutputWriter::finish()
{
    close();
}

/**
 * @brief Triggered when socket is ready to be read.
 */
void ProxySocketOutputWriter::readRequest()
{
    ProxyRequest *request = new ProxyRequest(m_socket, this);

    if (!request->readFromSocket()) {
        MessageHelper::debug("Failed to read from socket.");
        finish();
        return;
    }

    createDownload(request);
}

void ProxySocketOutputWriter::close()
{
    if (m_socket) {
        if (m_socket->isOpen()) {
            m_socket->close();
        }

        m_socket->deleteLater();
        m_socket = NULL;

        ProxyOutputWriter::close();

        emit finished();
    }
}

void ProxySocketOutputWriter::read(QIODevice *ioDevice)
{
    if (m_socket == NULL || !m_socket->isOpen())
        return;

    if (!m_writtenToSocket) {
        m_writtenToSocket = true;

        QTextStream os(m_socket);
        os.setAutoDetectUnicode(true);

        os << "HTTP/1.0 "
           << m_proxyDownload->inputObject()->httpStatusCode()
           << " "
           << m_proxyDownload->inputObject()->httpStatusDescription()
           << "\r\n";
        ListOfStringPairs headers = m_proxyDownload->inputObject()->responseHeaders();
        for (int i = 0; i < headers.count(); ++i) {
            os << headers.at(i).first << ": " << headers.at(i).second << "\r\n";
        }
        os << "\r\n";
        os.flush();
//        if (reply->header(QNetworkRequest::ContentTypeHeader).toString().toLower().contains("text/html")) {
//            while (reply->canReadLine())
//            {
//                QString line = reply->readLine();
//                MessageHelper::debug(line);
//                m_socket->write(line.toLatin1());
//            }

//        }
    }

    m_socket->write(ioDevice->readAll());
}
