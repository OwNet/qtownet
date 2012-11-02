#ifndef MODULEINITIALIZER_H
#define MODULEINITIALIZER_H

#include <QObject>

class ModuleInitializer : public QObject
{
    Q_OBJECT

public:
    explicit ModuleInitializer(QObject *parent = 0);
    void init();
    
signals:
    
public slots:
    
};

#endif // MODULEINITIALIZER_H
