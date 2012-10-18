#ifndef PROXYSTATICINPUTOBJECT_H
#define PROXYSTATICINPUTOBJECT_H

#include "proxyinputobject.h"

class ProxyStaticInputObject : public ProxyInputObject
{
public:
    ProxyStaticInputObject(ProxyRequest *request, QObject *parent = 0);

    void readRequest();

    const QString httpStatusCode() { return QString::number(200); }
    const QString httpStatusDescription() { return "OK"; }

private:
};

#endif // PROXYSTATICINPUTOBJECT_H
