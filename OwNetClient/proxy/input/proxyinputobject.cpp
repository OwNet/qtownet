#include "proxyinputobject.h"

ProxyInputObject::ProxyInputObject(ProxyRequest *request, QObject *parent) :
    QObject(parent), m_readHeaders(false), m_request(request)
{
}
