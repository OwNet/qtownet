#include "sharedfilesservice.h"

#include "irequest.h"
#include "sharedfilesmanager.h"
#include "iproxyconnection.h"
#include "isession.h"
#include "sharedfilesmanager.h"
#include "portalhelper.h"
#include "irouter.h"

void SharedFilesService::init(IRouter *router)
{
    router->addRoute("/allPagesCount")->on(IRequest::GET, ROUTE(allPagesCount));
    router->addRoute("/myPagesCount")->on(IRequest::GET, ROUTE(myPagesCount));
    router->addRoute("/my")->on(IRequest::GET, ROUTE(my));
}

IResponse *SharedFilesService::create(IRequest *req)
{
    IProxyConnection *proxyConnection = req->proxyConnection();
    if (!PortalHelper::isLoggedIn(proxyConnection))
        return req->response(IResponse::UNAUTHORIZED);

    SharedFilesManager manager(req->multipartContentTempFilePath(), proxyConnection);
    manager.saveFileToCache();

    return req->response(IResponse::OK);
}

IResponse *SharedFilesService::index(IRequest *req)
{
    IProxyConnection *proxyConnection = req->proxyConnection();
    if (!PortalHelper::isLoggedIn(proxyConnection))
        return req->response(IResponse::UNAUTHORIZED);

    SharedFilesManager manager(proxyConnection);
    return req->response(manager.listAvailableFiles(req->parameterValue("page").toInt(),
                                                    SharedFilesManager::AllFiles,
                                                    req->parameterValue("groupId").toUInt()));
}

IResponse *SharedFilesService::del(IRequest *req, const QString &uid)
{
    IProxyConnection *proxyConnection = req->proxyConnection();
    if (!PortalHelper::isLoggedIn(proxyConnection))
        return req->response(IResponse::UNAUTHORIZED);

    SharedFilesManager manager(proxyConnection);
    manager.removeFile(uid);

    return req->response(IResponse::OK);
}

IResponse *SharedFilesService::my(IRequest *req)
{
    IProxyConnection *proxyConnection = req->proxyConnection();
    if (!PortalHelper::isLoggedIn(proxyConnection))
        return req->response(IResponse::UNAUTHORIZED);

    SharedFilesManager manager(proxyConnection);
    return req->response(manager.listAvailableFiles(req->parameterValue("page").toInt(),
                                                    SharedFilesManager::MyFiles,
                                                    req->parameterValue("groupId").toUInt()));
}

IResponse *SharedFilesService::allPagesCount(IRequest *req)
{
    IProxyConnection *proxyConnection = req->proxyConnection();
    if (!PortalHelper::isLoggedIn(proxyConnection))
        return req->response(IResponse::UNAUTHORIZED);

    SharedFilesManager manager(proxyConnection);

    QVariantMap response;
    response.insert("pages",manager.numberOfPages(SharedFilesManager::AllFiles,
                    req->parameterValue("groupId").toUInt()));
    return req->response(response);
}

IResponse *SharedFilesService::myPagesCount(IRequest *req)
{
    IProxyConnection *proxyConnection = req->proxyConnection();
    if (!PortalHelper::isLoggedIn(proxyConnection))
        return req->response(IResponse::UNAUTHORIZED);

    SharedFilesManager manager(proxyConnection);

    QVariantMap response;
    response.insert("pages",manager.numberOfPages(SharedFilesManager::MyFiles,
                                                  req->parameterValue("groupId").toUInt()));
    return req->response(response);
}
