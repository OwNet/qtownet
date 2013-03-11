#ifndef ISESSION_H
#define ISESSION_H

#include <QVariant>

class ISession
{
public:
    virtual bool isLoggedIn() const = 0;
    virtual uint userId() const = 0;
    virtual QVariantMap availableClients() const = 0;
    virtual QString serverId() const = 0;
    virtual bool isServer() const = 0;
    virtual bool isOnline() const = 0;

    virtual void clear() = 0;

    virtual QVariant value(const QString &key) const = 0;
    virtual void setValue(const QString &key, const QVariant &value) = 0;
    virtual bool contains(const QString &key) const = 0;
};

#endif // ISESSION_H
