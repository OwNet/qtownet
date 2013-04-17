#ifndef SOCKETHANDLER_H
#define SOCKETHANDLER_H

#include <QObject>
#include <QTcpSocket>
#include <QMutex>

class QTcpSocket;
class RequestReader;
class ProxyHandler;
class QTimer;

class SocketHandler : public QObject
{
    Q_OBJECT
public:
    enum {
        Timeout = 5000
    };

    explicit SocketHandler(QTcpSocket *socketIn, QObject *parent = 0);

    bool startConnection();

    void readingAborted();
    void finishedReadingRequest();

    void write(const QByteArray &data);
    void writeStatusCodeAndDescription(int statusCode, const QByteArray &description);
    void writeHeaders(const QVariantMap &headers);

    void proxyHandlerFinished(ProxyHandler *handler);

    RequestReader *requestReader() const { return m_requestReader; }

public slots:
    void start();

private slots:
    /// input slots
    void inputDisconnected();
    void inputReadyRead();
    void inputError(QAbstractSocket::SocketError error);

    void socketTimeout();

signals:
    void finished();

private:
    void closeInput();

    QTcpSocket *m_socketIn;

    bool m_closed;
    int m_numProxyHandlers;

    RequestReader *m_requestReader;
    QMutex m_mutex;
    QTimer *m_timeoutTimer;
};

#endif // SOCKETHANDLER_H
