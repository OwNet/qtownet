#ifndef SAMPLEMODULE_H
#define SAMPLEMODULE_H

#include <QObject>

#include "imodule.h"

class SampleModule : public QObject, public IModule
{
    Q_OBJECT
    Q_INTERFACES(IModule)

public:
    QList<IRestService*> *restServices();
};

#endif // SAMPLEMODULE_H
