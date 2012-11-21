#ifndef SESSIONMODULE_H
#define SESSIONMODULE_H

#include <QVariantMap>

#include "irestmodule.h"

class SessionModule : public IRestModule
{
public:
    explicit SessionModule();

    QString name() const { return "session"; }

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
