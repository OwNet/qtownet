#ifndef PROXYINITIALIZER_H
#define PROXYINITIALIZER_H

#include <QObject>

class ProxyInitializer : public QObject
{
public:
    ProxyInitializer(QObject *parent = NULL);

    void init();
};

#endif // PROXYINITIALIZER_H
