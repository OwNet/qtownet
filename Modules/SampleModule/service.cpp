#include "service.h"

#include "irequest.h"

Service::Service(QObject *parent) :
    QObject(parent)
{
}

QVariant *Service::index(IBus *, IRequest *request)
{
    QVariantMap values;
    values.insert("module", request->module());
    return new QVariant(values);
}
