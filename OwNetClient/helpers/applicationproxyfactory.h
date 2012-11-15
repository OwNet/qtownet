#ifndef APPLICATIONPROXYFACTORY_H
#define APPLICATIONPROXYFACTORY_H

#include <QNetworkProxyFactory>

class ApplicationProxyFactory : public QNetworkProxyFactory
{
public:
    ApplicationProxyFactory();

    QList<QNetworkProxy> queryProxy ( const QNetworkProxyQuery & query = QNetworkProxyQuery() );

private:
    QNetworkProxy *m_externalProxy;
};

#endif // APPLICATIONPROXYFACTORY_H
