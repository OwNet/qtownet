#include "proxyinitializer.h"
#include "messagehelper.h"
#include "proxyrequest.h"

#include <QNetworkInterface>

ProxyInitializer::ProxyInitializer()
{
}

void ProxyInitializer::init()
{
    if (!m_proxyServer.listen(QHostAddress::Any, ProxyServer::PORT)) {
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
    MessageHelper::debug(QObject::tr("The server is running on\n\nIP: %1\nport: %2")
                               .arg(ipAddress).arg(m_proxyServer.serverPort()));
}
