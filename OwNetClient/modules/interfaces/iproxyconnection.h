#ifndef IPROXYCONNECTION_H
#define IPROXYCONNECTION_H

class ISession;
class IDatabaseUpdate;
class QSettings;

class IProxyConnection
{
public:
    virtual ISession *session() = 0;
    virtual IDatabaseUpdate *databaseUpdate() = 0;
    virtual QSettings *settings() = 0;
};

#endif // IPROXYCONNECTION_H
