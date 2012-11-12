#include "initializer.h"

Initializer::Initializer()
{
}

void Initializer::init()
{
    m_databaseInitializer.init();
    m_proxyInitializer.init();
    m_moduleInitializer.init();
    m_jobInitializer.init();
}
