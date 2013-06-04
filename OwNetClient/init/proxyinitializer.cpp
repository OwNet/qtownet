#include "proxyinitializer.h"
#include "messagehelper.h"
#include "proxyrequest.h"
#include "settings.h"
#include "websocket.h"
#include "applicationenvironment.h"

#include "httpserver.h"

#include <QNetworkInterface>
#include <QNetworkProxy>

ProxyInitializer::ProxyInitializer(QObject *parent)
    : QObject(parent)
{
}

void ProxyInitializer::init()
{
    Settings settings;
    settings.beginGroup("application");
    settings.setValue("readTimeout", 30*1000);
    settings.setValue("maxRequestSize", 32000);
    settings.setValue("minThreads", 10);

    if (ApplicationEnvironment().contains("CACHE_ONLY"))
        WebSocket::setCanDownloadFromWeb(false);

    new HttpServer;
}
