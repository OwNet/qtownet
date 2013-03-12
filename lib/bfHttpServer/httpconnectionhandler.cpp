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
      currentResponse(NULL)
{
    Q_ASSERT(settings!=0);
    Q_ASSERT(requestHandler!=0);
    this->settings=settings;
    this->requestHandler=requestHandler;
    currentRequest=0;
    busy = false;
    // execute signals in my own thread
    moveToThread(this);
    socket.moveToThread(this);
    readTimer.moveToThread(this);
    connect(&socket, SIGNAL(readyRead()), SLOT(read()));
    connect(&readTimer, SIGNAL(timeout()), SLOT(readTimeout()));
    readTimer.setSingleShot(true);
    qDebug("HttpConnectionHandler (%p): constructed", this);
    this->start();
}


HttpConnectionHandler::~HttpConnectionHandler() {
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


void HttpConnectionHandler::handleConnection(int socketDescriptor) {
    busy = true;
    Q_ASSERT(socket.isOpen()==false); // if not, then the handler is already busy

    //UGLY workaround - we need to clear writebuffer before reusing this socket
    //https://bugreports.qt-project.org/browse/QTBUG-28914
    socket.connectToHost("",0);
    socket.abort();

    if (!socket.setSocketDescriptor(socketDescriptor)) {
        qCritical("HttpConnectionHandler (%p): cannot initialize socket: %s", this,qPrintable(socket.errorString()));
        return;
    }
    // Start timer for read timeout
    int readTimeout=settings->value("readTimeout",10000).toInt();
    readTimer.start(readTimeout);
    // delete previous request
    delete currentRequest;
    currentRequest=0;
}


bool HttpConnectionHandler::isBusy() {
    return busy;
}

void HttpConnectionHandler::setBusy() {
    this->busy = true;
}


void HttpConnectionHandler::readTimeout() {
    qDebug("HttpConnectionHandler (%p): read timeout occured",this);

    //Commented out because QWebView cannot handle this.
    //socket.write("HTTP/1.1 408 request timeout\r\nConnection: close\r\n\r\n408 request timeout\r\n");

    socket.disconnectFromHost();
    delete currentRequest;
    currentRequest=0;
}


void HttpConnectionHandler::disconnected() {

    // Finalize sending the response if not already done
    if (currentResponse && !currentResponse->hasSentLastPart()) {
        currentResponse->write(QByteArray(),true);
        currentResponse->deleteLater();
        currentResponse = NULL;
    }

    socket.flush();
    socket.disconnectFromHost();

    // Prepare for next request
    delete currentRequest;
    currentRequest=0;

    socket.close();
    readTimer.stop();
    busy = false;
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
    while (socket.bytesAvailable() && currentRequest->getStatus()!=HttpRequest::complete && currentRequest->getStatus()!=HttpRequest::abort) {
        currentRequest->readFromSocket(socket);
        if (currentRequest->getStatus()==HttpRequest::waitForBody) {
            // Restart timer for read timeout, otherwise it would
            // expire during large file uploads.
            int readTimeout=settings->value("readTimeout",10000).toInt();
            readTimer.start(readTimeout);
        }
    }

    // If the request is aborted, return error message and close the connection
    if (currentRequest->getStatus()==HttpRequest::abort) {
        socket.write("HTTP/1.1 413 entity too large\r\nConnection: close\r\n\r\n413 Entity too large\r\n");
        socket.disconnectFromHost();
        delete currentRequest;
        currentRequest=0;
        disconnected();
        return;
    }

    // If the request is complete, let the request mapper dispatch it
    if (currentRequest->getStatus()==HttpRequest::complete) {
        readTimer.stop();
        currentResponse = new HttpResponse(&socket, this);
        connect(currentResponse, SIGNAL(finished()), this, SLOT(disconnected()));
        try {
            requestHandler->service(currentRequest, currentResponse);
        }
        catch (...) {
            qCritical("HttpConnectionHandler (%p): An uncaught exception occured in the request handler",this);
        }
    }
}
