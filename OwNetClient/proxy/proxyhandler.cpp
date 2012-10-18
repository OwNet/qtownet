#include "proxyhandler.h"

#include "messagehelper.h"
#include "proxyrequest.h"
#include "proxywebinputobject.h"
#include "proxystaticinputobject.h"

#include <QRegExp>
#include <QWidget>
#include <QDateTime>
#include <QNetworkAccessManager>
#include <QSemaphore>

ProxyHandler::ProxyHandler(QObject *parent)
    : QObject(parent), m_writtenToSocket(false)
{
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

    connect(m_socket, SIGNAL(readyRead()), this, SLOT(readRequest()));
}

void ProxyHandler::finishHandlingRequest()
{
    if (m_socket != NULL) {
        if (m_socket->isOpen()) {
            disconnect(m_socket, SIGNAL(readyRead()), this, SLOT(readRequest()));
            m_socket->close();
        }

        if (m_socket->state() == QTcpSocket::UnconnectedState) {
            delete m_socket;
            m_socket = NULL;
        }

        m_openSemaphore->release();
        requestFinished(this);
    }
}

void ProxyHandler::readRequest()
{
    ProxyRequest * request = new ProxyRequest(m_socket);

    request->readFromSocket();
    request->requestContentType();
    QString url = request->url();

    ProxyInputObject *inputObject = NULL;
    if (request->url().startsWith("http://static.ownet"))
        inputObject = new ProxyStaticInputObject(request, this);
    else
        inputObject = new ProxyWebInputObject(request, this);
    connect(inputObject, SIGNAL(finished()), this, SLOT(downloadFinished()));
    connect(inputObject, SIGNAL(readyRead(QIODevice*,ProxyInputObject*)), this, SLOT(readReply(QIODevice*,ProxyInputObject*)));

    inputObject->readRequest();
}

void ProxyHandler::readReply(QIODevice *ioDevice, ProxyInputObject *inputObject)
{
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
    finishHandlingRequest();
    emit finished();
}
