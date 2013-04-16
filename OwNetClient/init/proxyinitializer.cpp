#include "proxyinitializer.h"
#include "messagehelper.h"
#include "proxyrequest.h"
#include "settings.h"

#include "httpserver.h"

#include <QNetworkInterface>
#include <QNetworkProxy>

ProxyInitializer::ProxyInitializer(QObject *parent)
    : QObject(parent)
{
}

void ProxyInitializer::init()
{
    Settings *settings = new Settings();
    settings->beginGroup("application");
    settings->setValue("readTimeout", 30*1000);
    settings->setValue("maxRequestSize", 32000);
    settings->setValue("minThreads", 10);

    new HttpServer;
}
