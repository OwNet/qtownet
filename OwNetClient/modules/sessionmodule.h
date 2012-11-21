#ifndef SESSIONMODULE_H
#define SESSIONMODULE_H

#include <QObject>
#include <QVariantMap>
#include <QByteArray>
#include <imodule.h>

class SessionModule : public IModule
{
public:
    explicit SessionModule();

    QString url() const { return "session"; }

    // check whether user is logged in or not
    bool isLoggedIn();

private:

    // create session
    QByteArray *create(IBus *bus, IRequest *req);

    //delete element
    QByteArray *del(IBus *bus, IRequest *);

    QVariantMap m_sessionData;
};

#endif // SESSIONMODULE_H
