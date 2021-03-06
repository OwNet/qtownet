#ifndef IJOBACTION_H
#define IJOBACTION_H

#include <QObject>

class IProxyConnection;

class IJobAction : public QObject
{
public:
    virtual int interval() = 0;
    virtual void execute() = 0;
    virtual void setProxyConnection(IProxyConnection *proxyConnection) = 0;
    virtual bool createSeparateThread() const { return true; }
};

#endif // IJOBACTION_H
