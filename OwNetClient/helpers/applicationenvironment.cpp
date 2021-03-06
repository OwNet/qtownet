#include "applicationenvironment.h"

#include "settings.h"
#include "cachehelper.h"

#include <QDebug>

ApplicationEnvironment::ApplicationEnvironment(QObject *parent) :
    QObject(parent)
{
}

QString ApplicationEnvironment::value ( const QString & name, const QString & defaultValue) const
{
    return QProcessEnvironment::systemEnvironment().value(name, defaultValue);
}

bool ApplicationEnvironment::contains ( const QString & name ) const
{
    return QProcessEnvironment::systemEnvironment().contains(name);
}

void ApplicationEnvironment::init()
{
    if (contains("LISTEN_PORT")) {
        qDebug() << value("LISTEN_PORT");
        Settings::setTemporaryListenPort(value("LISTEN_PORT", "8081").toInt());
    }
    if (contains("DONT_USE_DB_FOR_CACHE")) {
        CacheHelper::setCanUseDatabase(false);
    }
    if (contains("DONT_WRITE_TO_CACHE")) {
        CacheHelper::setCanWriteToCache(false);
    }
}
