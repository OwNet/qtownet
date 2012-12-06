#include "moduleinitializer.h"

#include "imodule.h"
#include "irestservice.h"
#include "iservice.h"
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
    loadPlugins();
}

void ModuleInitializer::initModule(IModule *module)
{
    module->init(new ProxyConnection(this));

    QList<IService *> *services = module->services();
    if (services)
        foreach (IService *service, *services)
            RequestRouter::addRoute(service);

    QList<IRestService *> *restServices = module->restServices();
    if (restServices)
        foreach (IRestService *service, *restServices)
            RequestRouter::addRoute(service);

    QList<IJobAction *> *jobs = module->jobs();
    if (jobs)
        foreach (IJobAction *jobAction, *jobs)
            new ModuleJob(jobAction, this);
}

void ModuleInitializer::loadPlugins()
{
    QDir modulesDir = QDir(qApp->applicationDirPath());

#if defined(Q_OS_WIN)
    if (modulesDir.dirName().toLower() == "debug" || modulesDir.dirName().toLower() == "release")
        modulesDir.cdUp();
#endif

    modulesDir.cd("modules");
    MessageHelper::debug(QObject::tr("Loading modules from %1")
                         .arg(modulesDir.absolutePath()));

    foreach (QString fileName, modulesDir.entryList(QDir::Files)) {
        QPluginLoader loader(modulesDir.absoluteFilePath(fileName));
        QObject *plugin = loader.instance();

        if (plugin) {

            IModule *module = qobject_cast<IModule *>(plugin);
            if (module)
                initModule(module);
        }
    }
}
