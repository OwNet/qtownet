/**
  @file
  @author Stefan Frings
*/

#include "httpconnectionhandler.h"
#include "httprequest.h"
#include "httpresponse.h"
#include "proxyhandler.h"

#include <QTimer>
#include <QCoreApplication>

HttpConnectionHandler::HttpConnectionHandler(QSettings *settings)
    : QThread(),
      m_currentRequest(NULL),
      m_currentResponse(NULL),
      m_socket(NULL),
      m_socketDescriptor(0),
      m_proxyHandler(NULL)
{
    Q_ASSERT(settings!=0);
    this->m_settings=settings;
    // execute signals in my own thread
    moveToThread(this);
    m_readTimer.moveToThread(this);
    connect(&m_readTimer, SIGNAL(timeout()), SLOT(readTimeout()));
    m_readTimer.setSingleShot(true);
    qDebug("HttpConnectionHandler (%p): constructed", this);
    connect(this, SIGNAL(disposeHandler()), this, SLOT(quit()), Qt::QueuedConnection);
    connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
    connect(this, SIGNAL(startHandlingSig()), this, SLOT(handleConnection()), Qt::QueuedConnection);
    this->start();
}

void HttpConnectionHandler::run() {
    try {
        exec();
    }
    catch (...) {
        qCritical("HttpConnectionHandler (%p): an uncatched exception occured in the thread",this);
    }
    // Change to the main thread, otherwise deleteLater() would not work
    moveToThread(QCoreApplication::instance()->thread());
}


void HttpConnectionHandler::handleConnection() {
    m_socket = new QTcpSocket(this);
    connect(m_socket, SIGNAL(readyRead()), SLOT(read()));
    if (!m_socket->setSocketDescriptor(m_socketDescriptor)) {
        qCritical("HttpConnectionHandler (%p): cannot initialize socket: %s", this,qPrintable(m_socket->errorString()));
        return;
    }
    // Start timer for read timeout
    int readTimeout=m_settings->value("readTimeout",10000).toInt();
    m_readTimer.start(readTimeout);
}


void HttpConnectionHandler::readTimeout() {
    qDebug("HttpConnectionHandler (%p): read timeout occured",this);

    //Commented out because QWebView cannot handle this.
    //socket.write("HTTP/1.1 408 request timeout\r\nConnection: close\r\n\r\n408 request timeout\r\n");

    disconnected();
}


void HttpConnectionHandler::disconnected() {

    // Finalize sending the response if not already done
    if (m_currentResponse && !m_currentResponse->hasSentLastPart()) {
        m_currentResponse->write(QByteArray(),true);
        m_currentResponse->deleteLater();
        m_currentResponse = NULL;
    }

    m_socket->flush();
    m_socket->disconnectFromHost();

    m_currentRequest = NULL;

    m_socket->close();
    m_readTimer.stop();

    if (m_proxyHandler) {
        m_proxyHandler->deleteLater();
        m_proxyHandler = NULL;
    }

    emit disposeHandler();
}

void HttpConnectionHandler::read() {
#ifdef SUPERVERBOSE
    qDebug("HttpConnectionHandler (%p): read input",this);
#endif

    // Create new HttpRequest object if necessary
    if (!m_currentRequest) {
        m_currentRequest = new HttpRequest(m_settings, this);
    }

    // Collect data for the request object
    while (m_socket->bytesAvailable() && m_currentRequest->getStatus()!=HttpRequest::complete && m_currentRequest->getStatus()!=HttpRequest::abort) {
        m_currentRequest->readFromSocket(*m_socket);
        if (m_currentRequest->getStatus()==HttpRequest::waitForBody) {
            // Restart timer for read timeout, otherwise it would
            // expire during large file uploads.
            int readTimeout=m_settings->value("readTimeout",10000).toInt();
            m_readTimer.start(readTimeout);
        }
    }

    // If the request is aborted, return error message and close the connection
    if (m_currentRequest->getStatus()==HttpRequest::abort) {
        m_socket->write("HTTP/1.1 413 entity too large\r\nConnection: close\r\n\r\n413 Entity too large\r\n");
        m_socket->disconnectFromHost();
        delete m_currentRequest;
        m_currentRequest=0;
        disconnected();
        return;
    }

    // If the request is complete, let the request mapper dispatch it
    if (m_currentRequest->getStatus()==HttpRequest::complete) {
        m_readTimer.stop();
        m_currentResponse = new HttpResponse(m_socket, this);
        connect(m_currentResponse, SIGNAL(finished()), this, SLOT(disconnected()), Qt::QueuedConnection);
        try {
            m_proxyHandler = new ProxyHandler(this);
            m_proxyHandler->service(m_currentRequest, m_currentResponse);
        }
        catch (...) {
            qCritical("HttpConnectionHandler (%p): An uncaught exception occured in the request handler",this);
        }
    }
}


void HttpConnectionHandler::startHandling()
{
    emit startHandlingSig();
}
