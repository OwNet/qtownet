#include "proxyinputobject.h"

ProxyInputObject::ProxyInputObject(ProxyRequest *request, QObject *parent) :
    m_request(request), QObject(parent), m_readHeaders(false)
{
}
