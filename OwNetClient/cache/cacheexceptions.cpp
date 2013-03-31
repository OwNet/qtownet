#include "cacheexceptions.h"

#include "databaseselectquery.h"
#include "databaseupdatequery.h"

#include <QUrl>

CacheExceptions::CacheExceptions(QObject *parent) :
    QObject(parent)
{
    init();

    DatabaseUpdateQuery::registerListener(this);
}

void CacheExceptions::tableUpdated(IDatabaseUpdateQuery *query)
{
    QVariantList columns = query->columnsForListeners();
    for (int i = 0; i < columns.count(); ++i) {
        QVariantMap columnsMap = columns.at(i).toMap();

        if (query->type() == IDatabaseUpdateQuery::InsertOrUpdate)
            m_cacheExceptions.insert(columnsMap.value("host").toString(), true);
        else if (query->type() == IDatabaseUpdateQuery::Delete)
            m_cacheExceptions.remove(columnsMap.value("host").toString());
    }
}

/**
 * @brief Returns true if the url should not be cached
 * @param url Full url of the object
 * @return True is there is an exception
 */
bool CacheExceptions::containsExceptionFor(const QString &url) const
{
    return m_cacheExceptions.contains(getHostName(url));
}

void CacheExceptions::init()
{
    DatabaseSelectQuery query("cache_exceptions");
    while (query.next())
        m_cacheExceptions.insert(query.value("host").toString(), true);
}

QString CacheExceptions::getHostName(const QString &url)
{
    QUrl qurl(url);
    QStringList split = qurl.host().split(".");

    while (split.count() > 2)
        split.removeFirst();

    return split.join(".");
}
