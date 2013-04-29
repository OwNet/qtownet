#include "sharedfilesservice.h"

#include "irequest.h"
#include "multipartformreader.h"
#include "iproxyconnection.h"
#include "sharedfilesmanager.h"

void SharedFilesService::init(IRouter *router)
{
}

IResponse *SharedFilesService::create(IRequest *req)
{
    IProxyConnection *proxyConnection = req->proxyConnection();
    SharedFilesManager manager(req->multipartContentTempFilePath(), proxyConnection);
    manager.saveFileToCache();

    return req->response(IResponse::OK);
}
