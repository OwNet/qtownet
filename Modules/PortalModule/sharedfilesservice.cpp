#include "sharedfilesservice.h"

#include "irequest.h"
#include "sharedfilesmanager.h"
#include "iproxyconnection.h"
#include "isession.h"
#include "sharedfilesmanager.h"
#include "portalhelper.h"

void SharedFilesService::init(IRouter *router)
{
}

IResponse *SharedFilesService::create(IRequest *req)
{
    IProxyConnection *proxyConnection = req->proxyConnection();
    if (!PortalHelper::isLoggedIn(proxyConnection))
        return req->response(IResponse::UNAUTHORIEZED);

    SharedFilesManager manager(req->multipartContentTempFilePath(), proxyConnection);
    manager.saveFileToCache();

    return req->response(IResponse::OK);
}

IResponse *SharedFilesService::index(IRequest *req)
{
    IProxyConnection *proxyConnection = req->proxyConnection();
    if (!PortalHelper::isLoggedIn(proxyConnection))
        return req->response(IResponse::UNAUTHORIEZED);

    SharedFilesManager manager(proxyConnection);
    return req->response(manager.listAvailableFiles());
}

IResponse *SharedFilesService::del(IRequest *req, const QString &uid)
{
    IProxyConnection *proxyConnection = req->proxyConnection();
    if (!PortalHelper::isLoggedIn(proxyConnection))
        return req->response(IResponse::UNAUTHORIEZED);

    SharedFilesManager manager(proxyConnection);
    manager.removeFile(uid);
}
