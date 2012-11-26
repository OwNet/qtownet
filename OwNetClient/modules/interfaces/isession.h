#ifndef ISESSION_H
#define ISESSION_H

#include <QVariant>

class ISession
{
public:
    virtual bool isLoggedIn() const = 0;
    virtual void clear() = 0;

    virtual QVariant value(const QString &key) const = 0;
    virtual void setValue(const QString &key, const QVariant &value) = 0;
};

#endif // ISESSION_H
