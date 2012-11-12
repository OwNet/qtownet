#ifndef DATABASEMODULE_H
#define DATABASEMODULE_H

#include <imodule.h>
#include <QByteArray>
#include "ibus.h"

class DatabaseModule : public IModule
{
    Q_OBJECT
public:
    explicit DatabaseModule(QObject *parent = NULL);

   /* // create element
    QByteArray* create(IBus *bus, ProxyRequest *req){return new QByteArray;}

    // show element
    QByteArray* show( IBus *bus, ProxyRequest *req){return new QByteArray;}

    //delete element
    QByteArray* del( IBus *bus, ProxyRequest *req){return new QByteArray;}

    //edit element
    QByteArray* edit( IBus *bus, ProxyRequest *req){return new QByteArray;}

    QByteArray* index( IBus *bus,  ProxyRequest *req){return new QByteArray;}
*/

signals:

public slots:

private:


};

#endif // DATABASEMODULE_H
