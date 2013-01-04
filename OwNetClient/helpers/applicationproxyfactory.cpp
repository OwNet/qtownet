#include "applicationproxyfactory.h"
#include "applicationenvironment.h"
#include "messagehelper.h"
#include "settings.h"

/**
 * @brief Initialize proxy factory using configuration options.
 */
ApplicationProxyFactory::ApplicationProxyFactory()
{
    Settings settings;
    QString hostName = settings.value("application/proxy_host_name", "").toString();
    int port = settings.value("application/proxy_port", 8081).toInt();

    if (hostName != "")
    {
        MessageHelper::debug(QObject::tr("Setting external proxy to %1:%2")
                             .arg(hostName).arg(port));

        m_externalProxy = new QNetworkProxy();
        m_externalProxy->setType(QNetworkProxy::HttpProxy);
        m_externalProxy->setHostName(hostName);
        m_externalProxy->setPort(port);
    }
    else
        m_externalProxy = NULL;
}

/**
 * @brief Select proxy for request.
 */
QList<QNetworkProxy> ApplicationProxyFactory::queryProxy ( const QNetworkProxyQuery & query)
{
    QList<QNetworkProxy> proxies;

    if (m_externalProxy)
    {
        if (query.queryType() == QNetworkProxyQuery::UrlRequest)
            proxies.append(*m_externalProxy);
        else
            proxies.append(QNetworkProxy::NoProxy);
    }
    else
        proxies.append(QNetworkProxy::NoProxy);

    return proxies;
}
