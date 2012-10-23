#ifndef INITIALIZER_H
#define INITIALIZER_H

#include "databaseinitializer.h"
#include "proxyinitializer.h"

class Initializer
{
public:
    Initializer();

    void init();

private:
    ProxyInitializer m_proxyInitializer;
    DatabaseInitializer m_databaseInitializer;
};

#endif // INITIALIZER_H
