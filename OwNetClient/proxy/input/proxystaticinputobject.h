#ifndef PROXYSTATICINPUTOBJECT_H
#define PROXYSTATICINPUTOBJECT_H

#include "proxyinputobject.h"

class ProxyStaticInputObject : public ProxyInputObject
{
public:
    ProxyStaticInputObject(ProxyRequest *request, QObject *parent = 0);

protected:
    void readRequest();

    InputType inputType() const { return LocalFile; }

private:
};

#endif // PROXYSTATICINPUTOBJECT_H
