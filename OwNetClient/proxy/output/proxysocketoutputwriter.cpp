#include "proxysocketoutputwriter.h"

#include "proxydownload.h"
#include "proxyrequest.h"
#include "messagehelper.h"
#include "proxyinputobject.h"
#include "proxyhandler.h"

#include <QTcpSocket>
#include <QFile>
#include <QSemaphore>

ProxySocketOutputWriter::ProxySocketOutputWriter(int socketDescriptor, ProxyHandler *proxyHandler) :
    ProxyOutputWriter(proxyHandler, proxyHandler), m_socketDescriptor(socketDescriptor), m_writtenToSocket(false), m_foundBody(false), m_socket(NULL)
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
    ProxyRequest *request = new ProxyRequest(m_socket, m_proxyHandler);

    if (!request->readFromSocket()) {
        MessageHelper::debug("Failed to read from socket.");
        finish();
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
        ListOfStringPairs headers = m_proxyDownload->inputObject()->responseHeaders();
        for (int i = 0; i < headers.count(); ++i) {
            if (headers.at(i).second == "chunked")
                continue;
            os << headers.at(i).first << ": " << headers.at(i).second << "\r\n";
        }
        os << "\r\n";
        os.flush();
    }
    QRegExp rx("(.*<body[^>]*>)(.*)");
    if (!m_foundBody && m_proxyDownload->inputObject()->contentType().toLower().contains("text/html")) {
        while (!ioDevice->atEnd()) {
            QByteArray lineBytes = ioDevice->readLine();
            QString line = QString::fromLatin1(lineBytes);
            if (rx.indexIn(line) != -1 && rx.capturedTexts().length() == 3) {
        //    if (line.contains("<body")) {


                QStringList listx = rx.capturedTexts();

                m_socket->write(listx.at(1).toLatin1());

                m_socket->write(QString("<script type=\"text/javascript\" src=\"http://static.ownet/inject.js\"></script>")
                                .toLatin1());
                m_socket->write(listx.at(2).toLatin1());
                m_foundBody = true;
            }
            else
            {
                m_socket->write(lineBytes);
            }
        }
    }
    else {
        m_foundBody = true;
        m_socket->write(ioDevice->readAll());
    }
}
