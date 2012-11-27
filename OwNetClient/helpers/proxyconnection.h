#ifndef PROXYCONNECTION_H
#define PROXYCONNECTION_H

#include <QObject>

#include "iproxyconnection.h"

class ProxyConnection : public QObject, public IProxyConnection
{
    Q_OBJECT
public:
    explicit ProxyConnection(QObject *parent = 0);

    ISession *session(QObject *parent = 0);
    IDatabaseUpdate *databaseUpdate(QObject *parent = 0);
    QSettings *settings(QObject *parent = 0);
    IDatabaseSettings *databaseSettings(QObject *parent = 0);
    IRequest *createRequest(IRequest::RequestType requestType, const QString &module, const QString &action = QString(), int id = -1, QObject *parent = 0);
};

#endif // PROXYCONNECTION_H
