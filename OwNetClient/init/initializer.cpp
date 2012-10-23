#include "initializer.h"
#include "proxyinitializer.h"

Initializer::Initializer()
{
}

void Initializer::init()
{
    m_databaseInitializer.init();
    m_proxyInitializer.init();
}
