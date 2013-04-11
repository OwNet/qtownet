#ifndef SOCKETHANDLER_H
#define SOCKETHANDLER_H

#include <QObject>
#include <QTcpSocket>

class QTcpSocket;

class SocketHandler : public QObject
{
    Q_OBJECT
public:
    explicit SocketHandler(QTcpSocket *socketIn, QObject *parent = 0);

    bool startConnection();

public slots:
    void start();

private slots:
    /// input slots
    void inputDisconnected();
    void inputReadyRead();
    void inputError(QAbstractSocket::SocketError error);

    /// output slots
    void outputConnectedToServer();
    void outputReadyRead();
    void outputDisconnected();
    void outputError(QAbstractSocket::SocketError error);

signals:
    void finished();

private:
    QString extractServer(const QString &fullUrl) const;
    QString getServerName(const QString &serverAndPort) const;
    int getPort(const QString &serverAndPort) const;

    void closeInput();
    void closeOutput();

    QTcpSocket *m_socketIn;
    QTcpSocket *m_socketOut;

    bool m_closed;
    bool m_isOutputConnected;
};

#endif // SOCKETHANDLER_H
