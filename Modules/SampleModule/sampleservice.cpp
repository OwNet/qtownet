#include "sampleservice.h"

#include "irequest.h"

SampleService::SampleService(IProxyConnection *proxyConnection, QObject *parent) :
    QObject(parent)
{
}

QVariant *SampleService::index(IBus *, IRequest *request)
{
    QVariantMap values;
    values.insert("module", request->service());
    return new QVariant(values);
}
