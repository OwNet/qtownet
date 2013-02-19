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
    int port = Settings().value("application/listen_port", QString::number(ProxyRequest::Port)).toInt();
    settings->setValue("port", port);
    new HttpListener(settings, new ProxyRequestMapper());
}
