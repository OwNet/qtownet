#ifndef MODULEHELPERS_H
#define MODULEHELPERS_H

#include "../OwNetClient/modules/interfaces/imodule.h"

class ModuleHelpers
{
public:
    static IModule *loadModule(QString);
};

#endif // MODULEHELPERS_H
