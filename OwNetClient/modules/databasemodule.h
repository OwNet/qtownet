#ifndef DATABASEMODULE_H
#define DATABASEMODULE_H

#include "irestservice.h"
#include "ibus.h"

class DatabaseModule : public IRestService
{
public:
    explicit DatabaseModule();

    QString name() const { return "db"; }
};

#endif // DATABASEMODULE_H
