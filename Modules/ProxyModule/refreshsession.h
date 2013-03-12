#ifndef REFRESHSESSION_H
#define REFRESHSESSION_H

#include <QObject>

class IProxyConnection;

class RefreshSession : public QObject
{
    Q_OBJECT
public:
    explicit RefreshSession(IProxyConnection *proxyConnection, QObject *parent = 0);

    void start();
    void stop();

private slots:
    void timeout();

private:
    IProxyConnection *m_proxyConnection;
};

#endif // REFRESHSESSION_H
