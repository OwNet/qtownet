#include "initializer.h"
#include "proxyinitializer.h"
#include "proxyrequestbus.h"
#include "moduleinitializer.h"

Initializer::Initializer()
{
}

void Initializer::init()
{
    m_databaseInitializer.init();
    m_proxyInitializer.init();
    m_moduleInitializer.init();
}
