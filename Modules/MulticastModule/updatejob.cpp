#include "updatejob.h"

UpdateJob::UpdateJob()
    : m_protocol(NULL)
{
}

void UpdateJob::execute()
{
    m_protocol->update();
}

void UpdateJob::setProxyConnection(IProxyConnection *proxyConnection)
{
    m_protocol = new MulticastProtocol(proxyConnection, this);
}
