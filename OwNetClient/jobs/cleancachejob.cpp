#include "cleancachejob.h"

#include "cachefolder.h"
#include "webdownloadsmanager.h"
#include "gdsfclock.h"
#include "databaseupdatequery.h"
#include "databaseselectquerywheregroup.h"
#include "databasesettings.h"

#include <QSqlQuery>
#include <QDateTime>
#include <QSqlRecord>
#include <QVariant>

CleanCacheJob::CleanCacheJob(QObject *parent)
    : Job(60 * 1000, parent)
{
}

void CleanCacheJob::execute()
{
    if (!m_activeMutex.tryLock())
        return;

    clean();

    m_activeMutex.unlock();
}

void CleanCacheJob::clean()
{
    CacheFolder cacheFolder;
    long size = cacheFolder.size();
    long maximumSize = cacheFolder.maximumSize();
    QStringList idsToDelete;
    double lastEvictedClock = -1;
    QString myClientId = DatabaseSettings().clientId();

    if (size > maximumSize + cacheFolder.cacheReserveSize()) {
        QSqlQuery query;
        query.prepare("SELECT * FROM caches WHERE date_updated < :date_updated ORDER BY access_value LIMIT 100");
        query.bindValue("date_updated", QDateTime::currentDateTime().addSecs(-600).toString(Qt::ISODate));

        if (query.exec()) {
            while (size > maximumSize && query.next()) {
                int id = query.value(query.record().indexOf("id")).toInt();

                DatabaseUpdateQuery deleteClientCaches("client_caches", DatabaseUpdateQuery::Delete);
                IDatabaseSelectQueryWhereGroup *where = deleteClientCaches.whereGroup(IDatabaseSelectQuery::And);
                where->where("cache_id", id);
                where->where("client_id", myClientId);
                deleteClientCaches.executeQuery();

                for (int i = 0; i < query.value(query.record().indexOf("num_parts")).toInt(); ++i) {
                    QFile *file = cacheFolder.cacheFile(id, i);

                    if (file->exists())
                        file->remove();
                    delete file;
                }
                size -= query.value(query.record().indexOf("size")).toLongLong();
                idsToDelete.append(QString::number(id));

                lastEvictedClock = query.value(query.record().indexOf("access_value")).toDouble();
            }
        }

        if (lastEvictedClock > 0) {
            WebDownloadsManager::instance()->gdsfClock()->setLastClock(lastEvictedClock);
        }

        if (idsToDelete.count()) {
            QSqlQuery query;
            query.prepare(QString("DELETE FROM caches WHERE id IN (%1)")
                          .arg(idsToDelete.join(", ")));
            query.exec();
        }
    }
}
