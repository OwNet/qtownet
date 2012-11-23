#ifndef COMMUNICATIONINITIALIZER_H
#define COMMUNICATIONINITIALIZER_H

class HeartbeatJob;
class HeartbeatServer;

class CommunicationInitializer
{
public:
    explicit CommunicationInitializer();
    void init();
    
private:
    HeartbeatJob *m_heartbeatJob;
    HeartbeatServer *m_heartbeatServer;
};

#endif // COMMUNICATIONINITIALIZER_H
