#ifndef SAMPLEMODULE_H
#define SAMPLEMODULE_H

#include <QObject>
#include <QVariantMap>

#include "modules/irestmodule.h"

class SessionModule : public QObject, public IRestModule
{
    Q_OBJECT
    Q_INTERFACES(IRestModule)

public:
    QString name() const { return "session"; }

    QVariant *create(IBus *bus, IRequest *req);

    QVariant *del(IBus *bus, IRequest *);
};

#endif // SAMPLEMODULE_H
