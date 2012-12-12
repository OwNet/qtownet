#include "sampleservice.h"

#include "irequest.h"

SampleService::SampleService(IProxyConnection *proxyConnection, QObject *parent) :
    QObject(parent)
{
}

IResponse *SampleService::index(IRequest *req)
{
    QVariantMap values;
    values.insert("module", req->service());

    return req->response(QVariant(values));
}
