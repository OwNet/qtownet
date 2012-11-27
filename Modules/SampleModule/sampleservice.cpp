#include "sampleservice.h"

#include "irequest.h"

SampleService::SampleService(QObject *parent) :
    QObject(parent)
{
}

QVariant *SampleService::index(IBus *, IRequest *request)
{
    QVariantMap values;
    values.insert("module", request->module());
    return new QVariant(values);
}
