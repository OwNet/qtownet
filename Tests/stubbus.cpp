#include "stubbus.h"

StubBus::StubBus(QObject *parent) :
    QObject(parent)
{
}

QByteArray* StubBus::callModule(IRequest *req)
{
    return NULL;
}

void StubBus::setHttpStatus(int code, const QString &description)
{
}

ISession *StubBus::session()
{
    return NULL;
}

IDatabaseUpdate *StubBus::databaseUpdate()
{
    return NULL;
}

QSettings *StubBus::settings()
{
    return NULL;
}
