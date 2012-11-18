#ifndef GROUPMODULE_H
#define GROUPMODULE_H

#include <QObject>
#include <imodule.h>
#include <ibus.h>
#include <QByteArray>

class GroupModule : public IModule
{
    Q_OBJECT
public:
    explicit GroupModule(QObject *parent = 0);



signals:

public slots:

private:

    // create element
    QByteArray* create(IBus *bus, ProxyRequest *req);

    // show element
    QByteArray* show( IBus *bus, ProxyRequest *req);

    //delete element
    QByteArray* del( IBus *bus, ProxyRequest *req);

    //edit element
    QByteArray* edit( IBus *bus, ProxyRequest *req);

    QByteArray* index( IBus *bus,  ProxyRequest *req);
};

#endif // GROUPMODULE_H
