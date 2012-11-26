#ifndef IJOBACTION_H
#define IJOBACTION_H

class IJobAction
{
public:
    virtual int interval() = 0;
    virtual void execute() = 0;
};

#endif // IJOBACTION_H
