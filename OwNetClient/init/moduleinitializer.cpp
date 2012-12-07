#include "moduleinitializer.h"

#include "imodule.h"
#include "irestservice.h"
#include "iservice.h"
#include "databasemodule.h"
#include "requestrouter.h"
#include "messagehelper.h"
#include "proxyconnection.h"
#include "ijobaction.h"
#include "modulejob.h"

#include <QDir>
#include <QPluginLoader>
#include <QApplication>
#include <QList>

ModuleInitializer::ModuleInitializer(QObject *parent) :
    QObject(parent)
{
}

void ModuleInitializer::init()
{
    // here have to be all the used modules

//    RequestRouter::addRoute(new DatabaseModule());

    loadPlugins();
}

void ModuleInitializer::initModule(IModule *module)
{
    module->init(new ProxyConnection(this));
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
        MessageHelper::debug(QObject::tr("Loading %1")
                             .arg(modulesDir.absoluteFilePath(fileName)));

        QPluginLoader loader(modulesDir.absoluteFilePath(fileName));
        QObject *plugin = loader.instance();

        if (plugin) {
            IModule *module = qobject_cast<IModule *>(plugin);
            if (module)
                initModule(module);
        }
    }
}
