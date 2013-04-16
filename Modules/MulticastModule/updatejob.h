#ifndef UPDATEJOB_H
#define UPDATEJOB_H

#include "ijobaction.h"
#include "multicastprotocol.h"

class UpdateJob : public IJobAction
{
    Q_OBJECT
public:
    explicit UpdateJob();

    int interval() { return 1 * 1000; }
    void execute();
    void setProxyConnection(IProxyConnection *proxyConnection);

private:
    MulticastProtocol *m_protocol;
    
};

#endif // UPDATEJOB_H
