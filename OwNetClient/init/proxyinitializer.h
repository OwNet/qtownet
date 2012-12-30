#ifndef PROXYINITIALIZER_H
#define PROXYINITIALIZER_H

#include "proxyserver.h"

#include <QObject>

class ProxyInitializer : public QObject
{
public:
    ProxyInitializer(QObject *parent = NULL);

    void init();

private:
    ProxyServer m_proxyServer;
};

#endif // PROXYINITIALIZER_H
