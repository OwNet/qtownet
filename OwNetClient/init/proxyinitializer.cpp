#include "proxyinitializer.h"
#include "messagehelper.h"
#include "proxyrequest.h"
#include "settings.h"

#include "httplistener.h"
#include "proxyrequestmapper.h"

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
    new HttpListener(settings, new ProxyRequestMapper());
}
