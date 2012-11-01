#ifndef INITIALIZER_H
#define INITIALIZER_H

#include "databaseinitializer.h"
#include "proxyinitializer.h"
#include "moduleinitializer.h"

class Initializer
{
public:
    Initializer();

    void init();

private:
    ProxyInitializer m_proxyInitializer;
    DatabaseInitializer m_databaseInitializer;
    ModuleInitializer m_moduleInitializer;
};

#endif // INITIALIZER_H
