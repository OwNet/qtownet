#ifndef SYNCMODULE_H
#define SYNCMODULE_H

#include <QObject>

#include "imodule.h"

class SyncModule : public QObject, public IModule
{
    Q_OBJECT
    Q_INTERFACES(IModule)

public:
};

#endif // SYNCMODULE_H
