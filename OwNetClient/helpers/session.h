#ifndef SESSION_H
#define SESSION_H

#include <QVariantMap>

#include "isession.h"

class Session : public QObject, public ISession
{
    Q_OBJECT

public:
    Session(QObject *parent = NULL);

    void setValue(const QString &key, const QVariant &value);
    QVariant value(const QString &key) const;
    bool contains(const QString &key) const;

    void clear();

    bool isLoggedIn() const;
    uint userId() const;
    QVariantMap availableClients() const;
    QString serverId() const;
    bool isServer() const;
    bool isOnline() const;
    bool isRefreshSession() const;

private:
    static QVariantMap *m_sessionData;
};

#endif // SESSION_H
