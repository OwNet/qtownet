#include "proxyresponseoutputwriter.h"

#include "proxydownload.h"
#include "proxyrequest.h"
#include "messagehelper.h"
#include "proxyinputobject.h"
#include "proxyhandlersession.h"
#include "sockethandler.h"
#include "session.h"
#include "requestreader.h"

#include <QTcpSocket>
#include <QFile>
#include <QSemaphore>
#include <QRegularExpression>
#include <QBuffer>
#include <QUrl>

QMap<int, QString> *ProxyResponseOutputWriter::m_openRequests = new QMap<int, QString>();

ProxyResponseOutputWriter::ProxyResponseOutputWriter(SocketHandler *socketHandler, ProxyHandlerSession *proxyHandlerSession)
    : ProxyOutputWriter(proxyHandlerSession),
      m_socketHandler(socketHandler),
      m_hasWrittenResponseHeaders(false)
{
}

/**
 * @brief Opens the socket and starts the download, triggered from outside.
 */
void ProxyResponseOutputWriter::startDownload()
{
    ProxyRequest *request = new ProxyRequest(m_socketHandler->requestReader(), m_proxyHandlerSession);
    m_requestHashCode = request->hashCode();

    MessageHelper::debug(request->url());
    //m_openRequests->insert(m_requestHashCode, request->url());

    createDownload(request);
}

/**
 * @brief Return list of requests that are currently being processed.
 * @return List of open requests
 */
QList<QString> ProxyResponseOutputWriter::dumpOpenRequests()
{
    return m_openRequests->values();
}

/**
 * @brief Close socket and end download.
 */
void ProxyResponseOutputWriter::virtualClose()
{
}

/**
 * @brief Read data from a stream and write to socket.
 * @param ioDevice Stream of bytes
 */
void ProxyResponseOutputWriter::read(QIODevice *ioDevice)
{
    if (!m_proxyDownload->headersInBody() && !m_hasWrittenResponseHeaders) {
        ProxyInputObject *inputObject = m_proxyDownload->inputObject();

        m_hasWrittenResponseHeaders = true;
        if (!m_proxyDownload->inputObject()->httpStatusCode().toInt()) {
            MessageHelper::debug(tr("No status code in response %1").arg(inputObject->request()->url()));
            return;
        }

        m_socketHandler->writeStatusCodeAndDescription(m_proxyDownload->inputObject()->httpStatusCode().toInt(),
                              m_proxyDownload->inputObject()->httpStatusDescription().toUtf8());

        /// Remove the Content-Length header for html because it will change after injecting the scripts
        VariantMap responseHeaders = m_proxyDownload->inputObject()->responseHeaders();
        if (inputObject->addContentLengthHeader())
            responseHeaders.insert("Content-length", ioDevice->size());

        m_socketHandler->writeHeaders(responseHeaders);
    }
    m_hasWrittenResponseHeaders = true;
    m_socketHandler->write(ioDevice->readAll());
}


void ProxyResponseOutputWriter::error()
{
    if (!m_hasWrittenResponseHeaders) {
        if (!Session().isOnline()) {
            m_socketHandler->writeStatusCodeAndDescription(307, "Temporary Redirect");
            VariantMap responseHeaders;
            responseHeaders.insert("Location", QString("http://my.ownet/?url=%1")
                                   .arg(QString(QUrl::toPercentEncoding(m_socketHandler->requestReader()->url()))));
            responseHeaders.insert("Content-Type", "text/html");
            responseHeaders.insert("Content-Length", "0");
            m_socketHandler->writeHeaders(responseHeaders);
        }
    }
}
