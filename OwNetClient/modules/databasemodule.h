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

   QByteArray *processRequest(IBus *bus, ProxyRequest *req);

signals:

public slots:

};

#endif // DATABASEMODULE_H
