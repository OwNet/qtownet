#include "proxythread.h"

#include "messagehelper.h"
#include "proxyrequest.h"

#include <QRegExp>
#include <QWidget>
#include <QDateTime>
#include <QNetworkAccessManager>
#include <QMutex>

ProxyThread::ProxyThread(QTcpSocket *socket, QObject *parent)
    : QThread(parent), m_socket(socket), m_writtenToSocket(false)
{
    m_outputMutex = new QMutex();
    connect(this, SIGNAL(started()), this, SLOT(slotStarted()));

}

void ProxyThread::run()
{
    exec();
}

void ProxyThread::slotStarted()
{
    m_socket->moveToThread(this);
    ProxyRequest * request = new ProxyRequest(m_socket);

    request->readFromSocket();
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkReply *reply = NULL;
    MessageHelper::debug(request->networkRequest().url().toString());
    if (request->requestType() == ProxyRequest::GET)
        reply = manager->get(request->networkRequest());
    if (reply == NULL) {
        end();
        return;
    }

    connect(reply, SIGNAL(finished()), this, SLOT(slotDownloadFinished()));
    connect(reply, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
            this, SLOT(slotError(QNetworkReply::NetworkError)));
}

void ProxyThread::end()
{
    if (m_socket != NULL) {
        if (m_socket->isOpen())
            m_socket->close();

        if (m_socket->state() == QTcpSocket::UnconnectedState) {
            delete m_socket;
            m_socket = NULL;
        }
    }
}

void ProxyThread::slotReadyRead()
{
    if (m_socket == NULL || !m_socket->isOpen())
        return;

    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

    m_outputMutex->lock();
    if (!m_writtenToSocket) {
        m_writtenToSocket = true;

        QTextStream os(m_socket);
        os.setAutoDetectUnicode(true);
        QList<QNetworkReply::RawHeaderPair> headers = reply->rawHeaderPairs();

        os << "HTTP/1.0 200 Ok\r\n";
        for (int i = 0; i < headers.count(); ++i) {
            os << headers.at(i).first << ": " << headers.at(i).second << "\r\n";
        }
        os << "\r\n";
    }
    m_socket->write(reply->readAll());
    m_outputMutex->unlock();
}

void ProxyThread::slotError(QNetworkReply::NetworkError error)
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    MessageHelper::debug(reply->errorString());
}

void ProxyThread::slotDownloadFinished()
{
    end();
}
