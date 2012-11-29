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

QString StubBus::contentType() const
{
    return QString();
}

void StubBus::setContentType(const QString &value)
{
}
