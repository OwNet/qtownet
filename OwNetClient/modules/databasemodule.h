#ifndef DATABASEMODULE_H
#define DATABASEMODULE_H

#include "irestmodule.h"
#include "ibus.h"

class DatabaseModule : public IRestModule
{
public:
    explicit DatabaseModule();

    QString name() const { return "db"; }
};

#endif // DATABASEMODULE_H
