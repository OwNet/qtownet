#ifndef TESTMODULE_H
#define TESTMODULE_H

#include <imodule.h>
#include <QByteArray>
#include "ibus.h"

class TestModule : public IModule
{
    Q_OBJECT
public:
    explicit TestModule(QObject *parent = NULL);

   QByteArray* processRequest(IBus *bus, ProxyRequest *req);

signals:
    
public slots:
    
};

#endif // TESTMODULE_H
