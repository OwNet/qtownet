/**
  @file
  @author Stefan Frings
*/

#include "httpconnectionhandler.h"
#include "httpresponse.h"
#include <QTimer>
#include <QCoreApplication>

HttpConnectionHandler::HttpConnectionHandler(QSettings* settings, HttpRequestHandler* requestHandler)
    : QThread(),
      currentResponse(NULL),
      socket(NULL),
      m_socketDescriptor(0)
{
    Q_ASSERT(settings!=0);
    Q_ASSERT(requestHandler!=0);
    this->settings=settings;
    this->requestHandler=requestHandler;
    currentRequest=0;
    // execute signals in my own thread
    moveToThread(this);
    readTimer.moveToThread(this);
    connect(&readTimer, SIGNAL(timeout()), SLOT(readTimeout()));
    readTimer.setSingleShot(true);
    qDebug("HttpConnectionHandler (%p): constructed", this);
    connect(this, SIGNAL(disposeHandler()), this, SLOT(quit()));
    connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
    connect(this, SIGNAL(startHandlingSig()), this, SLOT(handleConnection()));
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
    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(readyRead()), SLOT(read()));
    if (!socket->setSocketDescriptor(m_socketDescriptor)) {
        qCritical("HttpConnectionHandler (%p): cannot initialize socket: %s", this,qPrintable(socket->errorString()));
        return;
    }
    // Start timer for read timeout
    int readTimeout=settings->value("readTimeout",10000).toInt();
    readTimer.start(readTimeout);
}


void HttpConnectionHandler::readTimeout() {
    qDebug("HttpConnectionHandler (%p): read timeout occured",this);

    //Commented out because QWebView cannot handle this.
    //socket.write("HTTP/1.1 408 request timeout\r\nConnection: close\r\n\r\n408 request timeout\r\n");

    socket->disconnectFromHost();
    delete currentRequest;
    currentRequest=0;

    emit disposeHandler();
}


void HttpConnectionHandler::disconnected() {

    // Finalize sending the response if not already done
    if (currentResponse && !currentResponse->hasSentLastPart()) {
        currentResponse->write(QByteArray(),true);
        currentResponse->deleteLater();
        currentResponse = NULL;
    }

    socket->flush();
    socket->disconnectFromHost();

    // Prepare for next request
    delete currentRequest;
    currentRequest=0;

    socket->close();
    readTimer.stop();

    emit disposeHandler();
}

void HttpConnectionHandler::read() {
#ifdef SUPERVERBOSE
    qDebug("HttpConnectionHandler (%p): read input",this);
#endif

    // Create new HttpRequest object if necessary
    if (!currentRequest) {
        currentRequest=new HttpRequest(settings);
    }

    // Collect data for the request object
    while (socket->bytesAvailable() && currentRequest->getStatus()!=HttpRequest::complete && currentRequest->getStatus()!=HttpRequest::abort) {
        currentRequest->readFromSocket(*socket);
        if (currentRequest->getStatus()==HttpRequest::waitForBody) {
            // Restart timer for read timeout, otherwise it would
            // expire during large file uploads.
            int readTimeout=settings->value("readTimeout",10000).toInt();
            readTimer.start(readTimeout);
        }
    }

    // If the request is aborted, return error message and close the connection
    if (currentRequest->getStatus()==HttpRequest::abort) {
        socket->write("HTTP/1.1 413 entity too large\r\nConnection: close\r\n\r\n413 Entity too large\r\n");
        socket->disconnectFromHost();
        delete currentRequest;
        currentRequest=0;
        disconnected();
        return;
    }

    // If the request is complete, let the request mapper dispatch it
    if (currentRequest->getStatus()==HttpRequest::complete) {
        readTimer.stop();
        currentResponse = new HttpResponse(socket, this);
        connect(currentResponse, SIGNAL(finished()), this, SLOT(disconnected()));
        try {
            requestHandler->service(currentRequest, currentResponse);
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
