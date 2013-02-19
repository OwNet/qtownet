#include "updatejob.h"

UpdateJob::UpdateJob(MulticastProtocol *protocol, QObject *parent)
    : m_protocol(protocol)
{
}

void UpdateJob::execute()
{
    m_protocol->update();
}
