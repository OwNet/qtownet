#ifndef INITIALIZER_H
#define INITIALIZER_H

#include "databaseinitializer.h"
#include "proxyinitializer.h"
#include "moduleinitializer.h"
#include "jobinitializer.h"

class Initializer
{
public:
    Initializer();

    void init();

private:
    ProxyInitializer m_proxyInitializer;
    DatabaseInitializer m_databaseInitializer;
    ModuleInitializer m_moduleInitializer;
    JobInitializer m_jobInitializer;
};

#endif // INITIALIZER_H
