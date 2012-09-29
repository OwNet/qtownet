#ifndef INITIALIZER_H
#define INITIALIZER_H

#include "proxyinitializer.h"

class Initializer
{
public:
    Initializer();

    void init();

private:
    ProxyInitializer m_proxyInitializer;
};

#endif // INITIALIZER_H
