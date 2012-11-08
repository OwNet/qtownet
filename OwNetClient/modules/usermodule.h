#ifndef USERMODULE_H
#define USERMODULE_H

#include <QObject>
#include <imodule.h>
#include <ibus.h>
#include <QByteArray>

class UserModule : public IModule
{
    Q_OBJECT
public:
    explicit UserModule(QObject *parent = 0);
    
    QByteArray* processRequest(IBus *bus, ProxyRequest *req);


signals:
    
public slots:

private:

    bool registerUser();
    
};

#endif // USERMODULE_H
