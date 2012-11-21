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

    void clear();

    bool isLoggedIn() const;

private:
    static QVariantMap *m_sessionData;
};

#endif // SESSION_H
