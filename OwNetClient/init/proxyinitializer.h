#ifndef PROXYINITIALIZER_H
#define PROXYINITIALIZER_H

#include "proxyserver.h"

#include <QObject>

class ProxyInitializer
{
public:
    ProxyInitializer();

    void init();

private:
    ProxyServer m_proxyServer;
};

#endif // PROXYINITIALIZER_H
