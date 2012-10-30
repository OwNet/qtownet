#ifndef INITIALIZER_H
#define INITIALIZER_H

#include "proxyinitializer.h"
#include "moduleinitializer.h"

class Initializer
{
public:
    Initializer();

    void init();

private:
    ProxyInitializer m_proxyInitializer;
    ModuleInitializer m_moduleInitializer;
};

#endif // INITIALIZER_H
