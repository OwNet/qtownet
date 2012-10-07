#include "proxyhandler.h"

#include "messagehelper.h"
#include "proxyrequest.h"

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
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkReply *reply = NULL;

    MessageHelper::debug(request->networkRequest().url().toString());

    if (request->requestType() == ProxyRequest::GET)
        reply = manager->get(request->networkRequest());
    if (reply == NULL) {
        finishHandlingRequest();
        return;
    }

    connect(reply, SIGNAL(finished()), this, SLOT(downloadFinished()));
    connect(reply, SIGNAL(readyRead()), this, SLOT(readReply()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error(QNetworkReply::NetworkError)));
}

void ProxyHandler::readReply()
{
    if (m_socket == NULL || !m_socket->isOpen())
        return;

    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

    if (!m_writtenToSocket) {
        m_writtenToSocket = true;

        QTextStream os(m_socket);
        os.setAutoDetectUnicode(true);
        QList<QNetworkReply::RawHeaderPair> headers = reply->rawHeaderPairs();

        os << "HTTP/1.0 "
           << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toString()
           << " "
           << reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString()
           << "\r\n";
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
    m_socket->write(reply->readAll());
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
