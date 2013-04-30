#ifndef PORTALHELPER_H
#define PORTALHELPER_H

#include <QObject>

class IProxyConnection;
class IDatabaseSelectQuery;

class PortalHelper : public QObject
{
    Q_OBJECT
public:
    explicit PortalHelper(QObject *parent = 0);

    static void addSalt( QString *password, QString *salt);
    static bool isLoggedIn(IProxyConnection *proxyConnection);
    static uint currentUserId(IProxyConnection *proxyConnection);
};

#endif // PORTALHELPER_H
