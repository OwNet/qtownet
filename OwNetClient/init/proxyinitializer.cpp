#include "proxyinitializer.h"
#include "messagehelper.h"
#include "proxyrequest.h"
#include "applicationenvironment.h"

#include <QNetworkInterface>
#include <QNetworkProxy>

ProxyInitializer::ProxyInitializer(QObject *parent)
    : QObject(parent)
{
}

void ProxyInitializer::init()
{
    // configure stub listening port
    int port = ApplicationEnvironment().value("OWNET_LISTEN_PORT",
                                              QString::number(ProxyServer::Port)).toInt();

    if (!m_proxyServer.listen(QHostAddress::Any, port)) {
        MessageHelper::error(QObject::tr("Failed to start server"),
                                QObject::tr("Unable to start the server: %1.")
                                .arg(m_proxyServer.errorString()));
        return;
    }

    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // use the first non-localhost IPv4 address
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
                ipAddressesList.at(i).toIPv4Address()) {
            ipAddress = ipAddressesList.at(i).toString();
            break;
        }
    }
    // if we did not find one, use IPv4 localhost
    if (ipAddress.isEmpty())
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
    MessageHelper::debug(QObject::tr("Proxy listens on external address %1:%2")
                               .arg(ipAddress).arg(m_proxyServer.serverPort()));
}
