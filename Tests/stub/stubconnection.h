#ifndef STUBCONNECTION_H
#define STUBCONNECTION_H

#include "../OwNetClient/modules/interfaces/iproxyconnection.h"

class StubConnection : public IProxyConnection
{
public:
    StubConnection();

    ISession *session(QObject *parent = 0);
    IDatabaseUpdate *databaseUpdate(QObject *parent = 0);
    QSettings *settings(QObject *parent = 0);
    IRequest *createRequest(IRequest::RequestType requestType, const QString &module, const QString &action = QString(), int id = -1, QObject *parent = 0);
};

#endif // STUBCONNECTION_H
