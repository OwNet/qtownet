#ifndef MODULEINITIALIZER_H
#define MODULEINITIALIZER_H

#include <QObject>

class IModule;

class ModuleInitializer : public QObject
{
    Q_OBJECT

public:
    explicit ModuleInitializer(QObject *parent = 0);
    void init();
    
private:
    void loadPlugins();
    void initModule(IModule *module);
};

#endif // MODULEINITIALIZER_H
