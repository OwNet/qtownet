#include "moduleinitializer.h"
#include "proxyrequestbus.h"
#include "qlist.h"

#include "imodule.h"
#include "testmodule.h"
#include "databasemodule.h"
#include "usermodule.h"

ModuleInitializer::ModuleInitializer(QObject *parent) :
    QObject(parent)
{
}

void ModuleInitializer::init()
{
    QList<IModule*> moduleList;

    // here have to be all the used modules
    moduleList.append(new TestModule());
    moduleList.append(new DatabaseModule());
    moduleList.append(new UserModule());

    for (int i = 0; i < moduleList.count(); i++) {
        ProxyRequestBus::registerModule(moduleList.at(i), moduleList.at(i)->url());
   }
}
