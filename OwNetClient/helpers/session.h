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
    QVariantMap availableClients() const;
    uint serverId() const;
    bool isServer() const;

private:
    static QVariantMap *m_sessionData;
};

#endif // SESSION_H
