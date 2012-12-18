#ifndef UPDATEJOB_H
#define UPDATEJOB_H

#include "ijobaction.h"
#include "multicastprotocol.h"

#include <QObject>

class UpdateJob : public QObject, public IJobAction
{
    Q_OBJECT
public:
    explicit UpdateJob(MulticastProtocol *protocol, QObject *parent = 0);

    int interval() { return 1 * 1000; };
    void execute();
    
signals:
    
public slots:

private:
    MulticastProtocol *m_protocol;
    
};

#endif // UPDATEJOB_H
