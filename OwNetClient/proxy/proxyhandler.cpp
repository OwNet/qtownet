#include "proxyhandler.h"

#include "messagehelper.h"
#include "proxyrequest.h"
#include "proxywebinputobject.h"
#include "proxystaticinputobject.h"
#include "proxyrequestbus.h"

#include <QRegExp>
#include <QWidget>
#include <QDateTime>
#include <QNetworkAccessManager>
#include <QSemaphore>
#include <QTimer>

ProxyHandler::ProxyHandler(QObject *parent)
    : QObject(parent), m_request(NULL), m_writtenToSocket(false)
{
    m_timeoutTimer = new QTimer(this);
    connect(m_timeoutTimer, SIGNAL(timeout()), this, SLOT(requestTimeout()));

    m_openSemaphore = new QSemaphore(1);
    connect(this, SIGNAL(start()), this, SLOT(handleRequest()));
}

ProxyHandler::~ProxyHandler()
{
    delete m_openSemaphore;
}

void ProxyHandler::setDescriptorAndStart(int desc) {
    m_openSemaphore->acquire();
    m_socketDescriptor = desc;
    emit start();
}

void ProxyHandler::handleRequest()
{
    m_socket = new QTcpSocket();
    m_socket->setSocketDescriptor(m_socketDescriptor);
    m_writtenToSocket = false;

    m_timeoutTimer->start(Timeout);

    connect(m_socket, SIGNAL(readyRead()), this, SLOT(readRequest()));
}

void ProxyHandler::requestTimeout()
{
    finishHandlingRequest();
}

void ProxyHandler::finishHandlingRequest()
{
    m_timeoutTimer->stop();
    if (m_socket) {
        if (m_socket->isOpen()) {
            disconnect(m_socket, SIGNAL(readyRead()), this, SLOT(readRequest()));
            m_socket->close();
        }

        m_socket->deleteLater();
        m_request->deleteLater();
        m_socket = NULL;
        m_request = NULL;

        m_openSemaphore->release();
        emit requestFinished(this);
    }
}

void ProxyHandler::readRequest()
{
    m_request = new ProxyRequest(m_socket, this);

    if (!m_request->readFromSocket()) {
        MessageHelper::debug("Failed to read from socket.");
        finish();
        return;
    }

    ProxyInputObject *inputObject = NULL;

    if (m_request->isStaticResourceRequest())
        inputObject = new ProxyStaticInputObject(m_request, m_request);
    else if (m_request->isLocalRequest())
        inputObject = new ProxyRequestBus(m_request, m_request);
    else
        inputObject = new ProxyWebInputObject(m_request, m_request);

    connect(inputObject, SIGNAL(finished()), this, SLOT(downloadFinished()));
    connect(inputObject, SIGNAL(readyRead(QIODevice*,ProxyInputObject*)), this, SLOT(readReply(QIODevice*,ProxyInputObject*)));

    inputObject->readRequest();
}

void ProxyHandler::readReply(QIODevice *ioDevice, ProxyInputObject *inputObject)
{
    m_timeoutTimer->stop();
    if (m_socket == NULL || !m_socket->isOpen())
        return;

    if (!m_writtenToSocket) {
        m_writtenToSocket = true;

        QTextStream os(m_socket);
        os.setAutoDetectUnicode(true);

        os << "HTTP/1.0 "
           << inputObject->httpStatusCode()
           << " "
           << inputObject->httpStatusDescription()
           << "\r\n";
        ListOfStringPairs headers = inputObject->responseHeaders();
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

    m_timeoutTimer->start(Timeout);
}

void ProxyHandler::error(QNetworkReply::NetworkError)
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    MessageHelper::debug(reply->errorString());
    finishHandlingRequest();
}

void ProxyHandler::downloadFinished()
{
    finishHandlingRequest();
}

void ProxyHandler::finish()
{
    MessageHelper::debug("2");
    finishHandlingRequest();
    emit finished();
}
