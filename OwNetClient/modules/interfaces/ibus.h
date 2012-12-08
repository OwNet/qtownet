#ifndef IBUS_H
#define IBUS_H

#include <QVariant>

class QByteArray;
class IRequest;
class ISession;
class IDatabaseUpdate;
class QSettings;

class IBus
{

public:
    virtual QString contentType() const = 0;
    virtual void setContentType(const QString &value) = 0;

    virtual void setHttpStatus(int code, const QString &description) = 0;
};

#endif // IBUS_H
