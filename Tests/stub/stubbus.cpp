#include "stubbus.h"

StubBus::StubBus(QObject *parent) :
    QObject(parent)
{
}

void StubBus::setHttpStatus(int code, const QString &description)
{
}

QVariant *StubBus::callModule(IRequest *req)
{
    return NULL;
}
