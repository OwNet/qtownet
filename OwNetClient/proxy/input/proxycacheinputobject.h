#ifndef PROXYCACHEINPUTOBJECT_H
#define PROXYCACHEINPUTOBJECT_H

#include "proxyinputobject.h"

class ProxyCacheInputObject : public ProxyInputObject
{
public:
    ProxyCacheInputObject(ProxyRequest *request, QObject *parent = 0);

    bool exists() { return m_exists; }

    InputType inputType() const { return Cache; }

protected:
    void readRequest();

private:
    bool m_exists;
    int m_numParts;
};

#endif // PROXYCACHEINPUTOBJECT_H
