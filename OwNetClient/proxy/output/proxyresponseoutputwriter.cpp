#include "proxyresponseoutputwriter.h"

#include "proxydownload.h"
#include "proxyrequest.h"
#include "messagehelper.h"
#include "proxyinputobject.h"
#include "proxyhandlersession.h"
#include "sockethandler.h"

#include <QTcpSocket>
#include <QFile>
#include <QSemaphore>
#include <QRegularExpression>

QMap<int, QString> *ProxyResponseOutputWriter::m_openRequests = new QMap<int, QString>();

ProxyResponseOutputWriter::ProxyResponseOutputWriter(SocketHandler *socketHandler, ProxyHandlerSession *proxyHandlerSession)
    : ProxyOutputWriter(proxyHandlerSession),
      m_socketHandler(socketHandler),
      m_hasWrittenResponseHeaders(false),
      m_foundBody(false)
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
    ProxyInputObject *inputObject = m_proxyDownload->inputObject();

    if (!inputObject->headersInBody() && !m_hasWrittenResponseHeaders) {
        m_hasWrittenResponseHeaders = true;
        if (!m_proxyDownload->inputObject()->httpStatusCode().toInt()) {
            MessageHelper::debug(tr("No status code in response %1").arg(inputObject->request()->url()));
            return;
        }

        m_socketHandler->writeStatusCodeAndDescription(m_proxyDownload->inputObject()->httpStatusCode().toInt(),
                              m_proxyDownload->inputObject()->httpStatusDescription().toUtf8());

        /// Remove the Content-Length header for html because it will change after injecting the scripts
        VariantMap responseHeaders = m_proxyDownload->inputObject()->responseHeaders();

        m_socketHandler->writeHeaders(responseHeaders);
    }
    m_socketHandler->write(ioDevice->readAll());
}
