#ifndef SHAREDFILESSERVICE_H
#define SHAREDFILESSERVICE_H

#include "iuidrestservice.h"

class IProxyConnection;

class SharedFilesService : public IUidRestService
{
public:
    void init(IRouter* router);

    QString name() const { return "files"; }

    IResponse *create(IRequest *req);
    IResponse *index(IRequest *req);
};

#endif // SHAREDFILESSERVICE_H
