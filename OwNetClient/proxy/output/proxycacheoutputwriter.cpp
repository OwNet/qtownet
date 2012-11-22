#include "proxycacheoutputwriter.h"

#include "proxydownload.h"
#include "cachefolder.h"
#include "proxyinputobject.h"
#include "proxyrequest.h"
#include "messagehelper.h"
#include "proxydownloads.h"
#include "gdsfclock.h"
#include "databaseupdate.h"
#include "proxycachefiledownloadpart.h"

#include <QFile>
#include <QIODevice>
#include <QDateTime>
#include <QSqlQuery>
#include <QSqlRecord>

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
    connectProxyDownload();
}

/**
 * @brief Close the remaining output file in cache and save to database if successful.
 */
void ProxyCacheOutputWriter::virtualClose()
{
    if (m_cacheFile)
        m_cacheFile->close();

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

    if (m_firstRead) {
        m_firstRead = false;
        m_url = m_request->url();
        m_requestHeaders = VariantMap(m_request->requestHeaders()).toJsonString();
        m_responseHeaders = m_proxyDownload->inputObject()->responseHeaders().toJsonString();
        m_statusCode = m_proxyDownload->inputObject()->httpStatusCode().toInt();
        m_statusDescription = m_proxyDownload->inputObject()->httpStatusDescription();
    }

    if (m_partSizeWritten > MaxFileSize)
        finishedWritingToCacheFile();

    if (!m_cacheFile)
        createCacheFile();

    m_cacheFile->write(ioDevice->readAll());
    long size = ioDevice->size();
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

    DatabaseUpdateQuery::EntryType entryType = DatabaseUpdateQuery::Insert;

    if (sqlQuery.exec() && sqlQuery.next()) {
        entryType = DatabaseUpdateQuery::Update;
        accessCount = sqlQuery.value(sqlQuery.record().indexOf("access_count")).toInt();
    }
    accessCount++;

    DatabaseUpdate update;
    IDatabaseUpdateQuery *query = update.createUpdateQuery("caches", entryType);
    query->setUpdateDates(true);
    query->setWhere("id", m_hashCode);
    query->setColumnValue("id", m_hashCode);
    query->setColumnValue("absolute_uri", m_url);
    query->setColumnValue("request_headers", m_requestHeaders);
    query->setColumnValue("response_headers", m_responseHeaders);
    query->setColumnValue("num_parts", m_numFileParts);
    query->setColumnValue("status_code", m_statusCode);
    query->setColumnValue("status_description", m_statusDescription);
    query->setColumnValue("size", m_sizeWritten);
    query->setColumnValue("access_value", ProxyDownloads::instance()->gdsfClock()->getGDSFPriority(accessCount, m_sizeWritten));

    update.execute();
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

    CacheFolder cacheFolder;
    for (int i = 0; i < m_numFileParts; ++i) {
        QFile *file = cacheFolder.cacheFile(m_hashCode, i);
        if (file->exists())
            file->remove();
        delete file;
    }
}

void ProxyCacheOutputWriter::finishedWritingToCacheFile()
{
    QString fileName = m_cacheFile->fileName();

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
