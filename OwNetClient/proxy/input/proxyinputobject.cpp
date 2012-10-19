#include "proxyinputobject.h"

ProxyInputObject::ProxyInputObject(ProxyRequest *request, QObject *parent) :
    QObject(parent), m_request(request)
{
}
