#include "proxycacheoutputwriter.h"

#include "proxydownload.h"
#include "cachefolder.h"
#include "proxyinputobject.h"
#include "proxyrequest.h"
#include "messagehelper.h"
#include "proxydownloads.h"
#include "gdsfclock.h"
#include "synceddatabaseupdatequery.h"
#include "proxycachefiledownloadpart.h"
#include "databasesettings.h"
#include "idatabaseselectquerywheregroup.h"
#include "cachehelper.h"

#include <QFile>
#include <QIODevice>
#include <QDateTime>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDateTime>

ProxyCacheOutputWriter::ProxyCacheOutputWriter(ProxyDownload *download, int downloadReaderId, ProxyHandlerSession *proxyHandlerSession)
    : ProxyOutputWriter(proxyHandlerSession),
      m_partSizeWritten(0),
      m_sizeWritten(0),
      m_numFileParts(0),
      m_fileStartedAtMemoryPart(0),
      m_failed(false),
      m_firstRead(true),
      m_currentMemoryPart(-1),
      m_cacheFile(NULL)
{
    m_downloadReaderId = downloadReaderId;
    m_proxyDownload = download;
    m_request = m_proxyDownload->inputObject()->request();
    m_hashCode = m_request->hashCode();
}

void ProxyCacheOutputWriter::saveToCache(uint hashCode, const QString &url, int numParts, qint64 size, int numAccesses)
{
	if (!CacheHelper::canUseDatabase())
		return;
    {
        DatabaseUpdateQuery query("caches", DatabaseUpdateQuery::InsertOrUpdate);
        query.setUpdateDates(true);
        query.singleWhere("id", hashCode);
        query.setColumnValue("id", hashCode);
        query.setColumnValue("absolute_uri", url);
        query.setColumnValue("num_parts", numParts);
        query.setColumnValue("size", size);
        query.setColumnValue("access_value", ProxyDownloads::instance()->gdsfClock()->getGDSFPriority(numAccesses, size));
        query.executeQuery();
    }
    {
        QString clientId = DatabaseSettings().clientId();
        SyncedDatabaseUpdateQuery query("client_caches");
        query.setUpdateDates(IDatabaseUpdateQuery::DateCreated);
        query.setColumnValue("client_id", clientId);
        query.setColumnValue("cache_id", hashCode);
        IDatabaseSelectQueryWhereGroup *where = query.whereGroup(IDatabaseSelectQuery::And);
        where->where("client_id", clientId);
        where->where("cache_id", hashCode);
        query.setGroupId(ISyncedDatabaseUpdateQuery::GroupCaches);
        query.setForceOperation(ProxyDownloads::instance()->containsCacheLocation(hashCode, clientId) ?
                                    DatabaseUpdateQuery::ForceUpdate : DatabaseUpdateQuery::ForceInsert);
        query.executeQuery();
    }
}

/**
 * @brief Close the remaining output file in cache and save to database if successful.
 */
void ProxyCacheOutputWriter::virtualClose()
{
    if (m_cacheFile) {
        m_cacheFile->flush();
        m_cacheFile->close();
    }

    if (!m_failed)
        save();
}

/**
 * @brief Read data from input and write to the current file in cache. Creates a new file if maximum size reached.
 * @param ioDevice
 */
void ProxyCacheOutputWriter::read(QIODevice *ioDevice)
{
    m_currentMemoryPart++;

    if (m_partSizeWritten > MaxFileSize)
        finishedWritingToCacheFile();

    QByteArray ba = ioDevice->readAll();

    if (m_firstRead) {
        m_firstRead = false;
        m_url = m_request->url();
        int pos = ba.indexOf("\n");
        ba.insert(pos+1, "X-OwNet-CachedOn: " + QDateTime::currentDateTime().toString(Qt::ISODate) + "\r\n");
    }

    long size = ba.size();
    if (size == 0)
        return;

    if (!m_cacheFile)
        createCacheFile();

    m_cacheFile->write(ba);
    m_partSizeWritten += size;
    m_sizeWritten += size;
}

/**
 * @brief Save cache info to database (i.e. url, request and response headers).
 * @return True if succesful
 */
void ProxyCacheOutputWriter::save()
{
    if (m_firstRead)
        return;

    QSqlQuery sqlQuery;
    sqlQuery.prepare("SELECT access_count FROM caches WHERE id = :id LIMIT 1");
    sqlQuery.bindValue(":id", m_hashCode);
    int accessCount = 0;

    if (sqlQuery.exec() && sqlQuery.next())
        accessCount = sqlQuery.value(sqlQuery.record().indexOf("access_count")).toInt();
    accessCount++;

    saveToCache(m_hashCode, m_url, m_numFileParts, m_sizeWritten, accessCount);
}

/**
 * @brief Triggered if input failed. Removes the data written in cache.
 */
void ProxyCacheOutputWriter::error()
{
    m_failed = true;
    if (m_cacheFile)
        m_cacheFile->close();
    m_cacheFile = NULL;

    if (m_numFileParts > 1 || m_partSizeWritten > 0) {
        CacheFolder cacheFolder;
        for (int i = 0; i < m_numFileParts; ++i) {
            QFile *file = cacheFolder.cacheFile(m_hashCode, i);
            if (file->exists())
                file->remove();
            delete file;
        }
    }
}

void ProxyCacheOutputWriter::finishedWritingToCacheFile()
{
    QString fileName = m_cacheFile->fileName();

    m_cacheFile->flush();
    m_cacheFile->close();
    delete m_cacheFile;
    m_cacheFile = NULL;

    if (m_proxyDownload)
        m_proxyDownload->replaceDownloadParts(new ProxyCacheFileDownloadPart(fileName, m_currentMemoryPart, parent()),
                                              m_fileStartedAtMemoryPart);
}

/**
 * @brief Creates a new output file to be written to in cache.
 */
void ProxyCacheOutputWriter::createCacheFile()
{
    CacheFolder cacheFolder;
    m_cacheFile = cacheFolder.cacheFile(m_hashCode, m_numFileParts, this);
    m_cacheFile->open(QIODevice::WriteOnly);
    m_fileStartedAtMemoryPart = m_currentMemoryPart;
    m_numFileParts++;
    m_partSizeWritten = 0;
}
