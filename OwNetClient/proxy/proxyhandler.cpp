#include "proxyhandler.h"

#include "messagehelper.h"
#include "proxyrequest.h"
#include "proxywebinputobject.h"
#include "proxystaticinputobject.h"
#include "proxyrequestbus.h"
#include "proxydownloads.h"
#include "proxydownload.h"
#include "proxysocketoutputwriter.h"
#include "proxyhandlersession.h"

#include <QRegExp>
#include <QWidget>
#include <QDateTime>
#include <QNetworkAccessManager>
#include <QSemaphore>
#include <QTimer>
#include <QApplication>

ProxyHandler::ProxyHandler(QObject *parent)
    : QObject(parent),
      m_proxyHandlerSession(NULL),
      m_timeoutTimer(NULL)
{
    connect(this, SIGNAL(start()), this, SLOT(handleRequest()), Qt::QueuedConnection);
}

/**
 * @brief Set socket descriptor handle to desc and trigger start.
 * @param desc Socket descriptor
 */
void ProxyHandler::setDescriptorAndStart(int desc) {
    m_socketDescriptor = desc;
    emit start();
}

/**
 * @brief Triggered by start signal after setting the descriptor.
 * Creates handler session and opens SocketOutputWriter, which starts the download.
 */
void ProxyHandler::handleRequest()
{
    m_proxyHandlerSession = new ProxyHandlerSession(this);
    connect(m_proxyHandlerSession, SIGNAL(allFinished()), this, SLOT(proxyHandlerSessionFinished()));

    m_timeoutTimer = new QTimer(m_proxyHandlerSession);
    connect(m_timeoutTimer, SIGNAL(timeout()), this, SLOT(requestTimeout()));

    ProxySocketOutputWriter *socketOutputWriter = new ProxySocketOutputWriter(m_socketDescriptor, m_proxyHandlerSession);
    connect(socketOutputWriter, SIGNAL(iAmActive()), this, SLOT(restartTimeout()));

    socketOutputWriter->startDownload();
    m_timeoutTimer->start(Timeout);
}

/**
 * @brief Request timed out.
 */
void ProxyHandler::requestTimeout()
{
    m_timeoutTimer->stop();

    if (m_proxyHandlerSession)
        m_proxyHandlerSession->forceQuitAll();
}

/**
 * @brief Restart timeot timer after SocketOutputWriter triggers iAmReady signal.
 */
void ProxyHandler::restartTimeout()
{
    if (m_timeoutTimer) {
        m_timeoutTimer->stop();
        m_timeoutTimer->start();
    }
}

/**
 * @brief Triggered from outside to force quit the current download.
 */
void ProxyHandler::finishHandling()
{
    if (m_proxyHandlerSession)
        m_proxyHandlerSession->forceQuitAll();
}

/**
 * @brief The current download and all dependent objects finished, disposes the current download
 * and emits requestFinished signal.
 */
void ProxyHandler::proxyHandlerSessionFinished()
{
    if (m_timeoutTimer) {
        m_timeoutTimer->stop();
        m_timeoutTimer = NULL;
    }

    if (m_proxyHandlerSession) {
        m_proxyHandlerSession->deleteLater();
        m_proxyHandlerSession = NULL;

        emit disposeThread();
    }
}
