#ifndef GROUPMODULE_H
#define GROUPMODULE_H

#include "irestmodule.h"
#include "ibus.h"

#include <QObject>
#include <QByteArray>

class GroupModule : public QObject, public IRestModule
{
    Q_OBJECT
public:
    explicit GroupModule(QObject *parent = 0);

    QString name() const { return "groups"; }

private:

    // create element
    QVariant* create(IBus *bus, IRequest *req);

    // show element
    QVariant* show( IBus *bus, IRequest *req);

    QVariant* index( IBus *bus,  IRequest *req);
};

#endif // GROUPMODULE_H
