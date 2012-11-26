#ifndef PROXYCONNECTION_H
#define PROXYCONNECTION_H

#include <QObject>

#include "iproxyconnection.h"

class ProxyConnection : public QObject, public IProxyConnection
{
    Q_OBJECT
public:
    explicit ProxyConnection(QObject *parent = 0);

    ISession *session();
    IDatabaseUpdate *databaseUpdate();
    QSettings *settings();
};

#endif // PROXYCONNECTION_H
