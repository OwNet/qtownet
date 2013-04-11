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
    int port = settings->value("listen_port", QString::number(ProxyRequest::Port)).toInt();
    settings->setValue("listen_port", port);
    settings->setValue("readTimeout", 30*1000);
    settings->setValue("maxRequestSize", 32000);
    settings->setValue("minThreads", 10);

    new HttpServer;
}
