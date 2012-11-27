#ifndef PREFETCHINGMODULE_H
#define PREFETCHINMODULE_H

#include <QObject>

#include "imodule.h"

class PrefetchJob;

class PrefetchingModule : public QObject, public IModule
{
    Q_OBJECT
    Q_INTERFACES(IModule)

public:
    void init(IProxyConnection *proxyConnection);
    QList<IService*> *services();
    QList<IJobAction*> *jobs();
    PrefetchJob* prefetchJob();

private:
    QList<IService*> m_services;
    QList<IJobAction*> m_jobs;
    PrefetchJob* m_prefetchJob;
};


#endif // PREFETCHINGMODULE_H
