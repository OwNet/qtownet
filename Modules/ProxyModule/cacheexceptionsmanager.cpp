#include "cacheexceptionsmanager.h"

#include "iproxyconnection.h"
#include "idatabaseupdatequery.h"
#include "idatabaseselectquery.h"
#include <QDebug>
#include <QUrl>
#include <QStringList>

CacheExceptionsManager::CacheExceptionsManager(IProxyConnection *proxyConnection, QObject *parent) :
    QObject(parent),
    m_proxyConnection(proxyConnection)
{
}

void CacheExceptionsManager::addException(const QString &url)
{
    qDebug() << url;
    QString hostName = getHostName(url);
    if (hostName.isEmpty())
        return;

    QObject parent;
    IDatabaseUpdateQuery *query = m_proxyConnection->databaseUpdateQuery("cache_exceptions", &parent);
    query->setColumnValue("url", url);
    query->setColumnValue("host", hostName);
    query->singleWhere("host", hostName);
    query->setUpdateDates(IDatabaseUpdateQuery::DateCreated);
    query->executeQuery();
}

void CacheExceptionsManager::removeException(const QString &url)
{
     qDebug() << url;
    QString hostName = getHostName(url);
    if (hostName.isEmpty())
        return;

    QObject parent;
    IDatabaseUpdateQuery *query = m_proxyConnection->databaseUpdateQuery("cache_exceptions", &parent);
    query->setType(IDatabaseUpdateQuery::Delete);
    query->singleWhere("host", hostName);
    query->executeQuery();
}

bool CacheExceptionsManager::isException(const QString &url)
{
     qDebug() << url;
    QObject parent;
    IDatabaseSelectQuery *query = m_proxyConnection->databaseSelect("cache_exceptions", &parent);
    query->singleWhere("host", getHostName(url));
    return query->first();
}

QString CacheExceptionsManager::getHostName(const QString &url)
{
     qDebug() << url;
    QUrl qurl(url);
    QStringList split = qurl.host().split(".");

    while (split.count() > 2)
        split.removeFirst();

    return split.join(".");
}
