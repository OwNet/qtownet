#include "proxysocketoutputwriter.h"

#include "proxydownload.h"
#include "proxysocketrequest.h"
#include "messagehelper.h"
#include "proxyinputobject.h"
#include "proxyhandlersession.h"

#include <QTcpSocket>
#include <QFile>
#include <QSemaphore>
#include <QRegularExpression>

QMap<int, QString> *ProxySocketOutputWriter::m_openRequests = new QMap<int, QString>();

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
 * @brief Return list of requests that are currently being processed.
 * @return List of open requests
 */
QList<QString> ProxySocketOutputWriter::dumpOpenRequests()
{
    return m_openRequests->values();
}

/**
 * @brief Triggered when socket is ready to be read.
 */
void ProxySocketOutputWriter::readRequest()
{
    ProxySocketRequest *request = new ProxySocketRequest(m_socket, m_proxyHandlerSession);

    if (!request->readFromSocket()) {
        MessageHelper::debug("Failed to read from socket.");
        forceQuit();
        return;
    }
    m_requestHashCode = request->hashCode();

    MessageHelper::debug(request->url());
    //m_openRequests->insert(m_requestHashCode, request->url());

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
        m_socket = NULL;
        //m_openRequests->remove(m_requestHashCode);
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

// "chunked encoding in header"
//        ListOfStringPairs headers = m_proxyDownload->inputObject()->responseHeaders();
//        for (int i = 0; i < headers.count(); ++i) {
//            if (headers.at(i).second == "chunked")
//                continue;
//            os << headers.at(i).first << ": " << headers.at(i).second << "\r\n";

        foreach (QString key, m_proxyDownload->inputObject()->responseHeaders().keys()) {
            os << key
               << ": "
               << m_proxyDownload->inputObject()->responseHeaders().value(key).toString()
               << "\r\n";

        }
        os << "\r\n";
        os.flush();
    }
    QRegularExpression rx("(.*<body[^>]*>)(.*)");
    ProxyInputObject *inputObject = m_proxyDownload->inputObject();
    if (!m_foundBody && !inputObject->request()->isLocalRequest() && inputObject->contentType().toLower().contains("text/html")) {
        while (!ioDevice->atEnd()) {
            QByteArray lineBytes = ioDevice->readLine();
            QString line = QString::fromLatin1(lineBytes);
            QRegularExpressionMatch match = rx.match(line);
            if (match.hasMatch() && match.capturedTexts().length() == 3) {
//            if (line.contains("<body")) {


                QStringList listx = match.capturedTexts();

                m_socket->write(listx.at(1).toLatin1());

                m_socket->write(QString("<script type=\"text/javascript\" src=\"http://inject.ownet/js/tabframeinject.js\"></script>")
                                .toLatin1());
                m_socket->write(listx.at(2).toLatin1());
                m_socket->write(lineBytes);
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
