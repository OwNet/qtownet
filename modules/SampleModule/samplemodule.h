#ifndef SAMPLEMODULE_H
#define SAMPLEMODULE_H

#include <QObject>

#include "modules/imodule.h"

class SampleModule : public QObject, public IModule
{
    Q_OBJECT
    Q_INTERFACES(IModule)

public:
    QString url() const { return "sample"; }

    QByteArray *index(IBus *, IRequest *request);
};

#endif // SAMPLEMODULE_H
