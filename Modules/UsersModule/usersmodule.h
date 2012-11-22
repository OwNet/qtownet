#ifndef USERSMODULE_H
#define USERSMODULE_H

#include <QObject>

#include "modules/irestmodule.h"

class UsersModule : public QObject, public IRestModule
{
    Q_OBJECT
    Q_INTERFACES(IRestModule)

public:
    QString name() const { return "users"; }

    QVariant *create(IBus *bus, IRequest *req);
    QVariant *show(IBus *bus, IRequest *req);
    QVariant *index(IBus *bus, IRequest *);
};

#endif // USERSMODULE_H
