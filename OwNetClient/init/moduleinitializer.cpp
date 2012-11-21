#include "moduleinitializer.h"
#include "proxyrequestbus.h"
#include "qlist.h"

#include "imodule.h"
#include "databasemodule.h"
#include "usermodule.h"
#include "sessionmodule.h"

#include <QDir>
#include <QPluginLoader>
#include <QApplication>

ModuleInitializer::ModuleInitializer(QObject *parent) :
    QObject(parent)
{
}

void ModuleInitializer::init()
{
    // here have to be all the used modules

    ProxyRequestBus::registerModule(new DatabaseModule());
    ProxyRequestBus::registerModule(new UserModule());
    ProxyRequestBus::registerModule(new SessionModule());

    loadPlugins();
}

void ModuleInitializer::loadPlugins()
{
    QDir modulesDir = QDir(qApp->applicationDirPath());

#if defined(Q_OS_WIN)
    if (modulesDir.dirName().toLower() == "debug" || modulesDir.dirName().toLower() == "release")
        modulesDir.cdUp();
#endif

    modulesDir.cd("modules");

    foreach (QString fileName, modulesDir.entryList(QDir::Files)) {
        QPluginLoader loader(modulesDir.absoluteFilePath(fileName));
        QObject *plugin = loader.instance();

        if (plugin) {
            IModule *module = qobject_cast<IModule *>(plugin);
            if (module)
                ProxyRequestBus::registerModule(module);
        }
    }
}
