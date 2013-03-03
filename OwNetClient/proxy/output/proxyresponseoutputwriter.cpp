#include "proxyresponseoutputwriter.h"

#include "proxydownload.h"
#include "proxyrequest.h"
#include "messagehelper.h"
#include "proxyinputobject.h"
#include "proxyhandlersession.h"
#include "httpresponse.h"

#include <QTcpSocket>
#include <QFile>
#include <QSemaphore>
#include <QRegularExpression>

QMap<int, QString> *ProxyResponseOutputWriter::m_openRequests = new QMap<int, QString>();

ProxyResponseOutputWriter::ProxyResponseOutputWriter(HttpRequest *request, HttpResponse *response, ProxyHandlerSession *proxyHandlerSession)
    : ProxyOutputWriter(proxyHandlerSession),
      m_request(request),
      m_response(response),
      m_hasWrittenResponseHeaders(false),
      m_foundBody(false)
{
}

/**
 * @brief Opens the socket and starts the download, triggered from outside.
 */
void ProxyResponseOutputWriter::startDownload()
{
    ProxyRequest *request = new ProxyRequest(m_request, m_proxyHandlerSession);
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
    if (!m_response->hasSentLastPart())
        m_response->write(QByteArray(), true);
}

/**
 * @brief Read data from a stream and write to socket.
 * @param ioDevice Stream of bytes
 */
void ProxyResponseOutputWriter::read(QIODevice *ioDevice)
{
    if (m_response->hasSentLastPart())
        return;

    if (!m_hasWrittenResponseHeaders) {
        m_hasWrittenResponseHeaders = true;
        m_response->setStatus(m_proxyDownload->inputObject()->httpStatusCode().toInt(),
                              m_proxyDownload->inputObject()->httpStatusDescription().toUtf8());

        foreach (QString key, m_proxyDownload->inputObject()->responseHeaders().keys())
            m_response->setHeader(key.toUtf8(), m_proxyDownload->inputObject()->responseHeaders().value(key).toByteArray());
    }
    QRegularExpression rx("(.*<body[^>]*>)(.*)");
    ProxyInputObject *inputObject = m_proxyDownload->inputObject();

    if (!m_foundBody &&
            !inputObject->request()->isLocalRequest() &&
            inputObject->contentType().toLower().contains("text/html")) {

        while (!ioDevice->atEnd()) {
            QByteArray lineBytes = ioDevice->readLine();
            QString line = QString::fromLatin1(lineBytes);
            QRegularExpressionMatch match = rx.match(line);

            if (match.hasMatch() && match.capturedTexts().length() == 3) {
                QStringList listx = match.capturedTexts();

                m_response->write(listx.at(1).toLatin1());

                m_response->write(QString("<script type=\"text/javascript\" src=\"http://inject.ownet/js/inject.js\"></script>")
                                .toLatin1());
                m_response->write(listx.at(2).toLatin1());
                m_response->write(lineBytes);
                m_foundBody = true;
            }
            else {
                m_response->write(lineBytes);
            }
        }
    }
    else {
        m_foundBody = true;
        m_response->write(ioDevice->readAll());
    }
}
