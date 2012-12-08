#include "modulehelpers.h"

#include <QDir>
#include <QCoreApplication>
#include <QPluginLoader>

IModule *ModuleHelpers::loadModule(QString name)
{
    QDir modulesDir = QDir(qApp->applicationDirPath());

#if defined(Q_OS_WIN)
    if (modulesDir.dirName().toLower() == "debug" || modulesDir.dirName().toLower() == "release")
        modulesDir.cdUp();
#endif

    modulesDir.cd("../OwNetClient/modules");

#if defined(Q_OS_WIN)
    QString filename = name.append(".dll");
#else
    QString filename = name.prepend("lib").append(".so");
#endif

    QPluginLoader loader(modulesDir.absoluteFilePath(filename));
    QObject *plugin = loader.instance();

    return qobject_cast<IModule *>(plugin);
}
