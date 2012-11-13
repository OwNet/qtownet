#include "proxycacheoutputwriter.h"

#include "proxydownload.h"
#include "cachefolder.h"
#include "proxyinputobject.h"
#include "proxyrequest.h"
#include "messagehelper.h"
#include "proxydownloads.h"
#include "gdsfclock.h"

#include <QFile>
#include <QIODevice>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDateTime>

ProxyCacheOutputWriter::ProxyCacheOutputWriter(ProxyDownload *download, ProxyHandler *proxyHandler, QObject *parent)
    : ProxyOutputWriter(proxyHandler, parent), m_partSizeWritten(0), m_sizeWritten(0), m_numParts(0), m_failed(false)
{
    m_proxyDownload = download;
    m_request = m_proxyDownload->inputObject()->request();
    createCacheFile();
    connectProxyDownload();
    m_downloadReaderId = m_proxyDownload->registerReader();
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
    if (m_partSizeWritten > MaxFileSize) {
        m_cacheFile->close();
        delete m_cacheFile;

        createCacheFile();
    }
    m_cacheFile->write(ioDevice->readAll());
    long size = ioDevice->size();
    m_partSizeWritten += size;
    m_sizeWritten += size;
}

/**
 * @brief Save cache info to database (i.e. url, request and response headers).
 * @return True if succesful
 */
bool ProxyCacheOutputWriter::save()
{
    bool update = false;

    QSqlQuery query;
    query.prepare("SELECT access_count FROM caches WHERE id = :id LIMIT 1");
    query.bindValue(":id", m_proxyDownload->hashCode());
    update = query.exec() && query.next();
    int accessCount = 0;

    if (!update) {
        query.prepare("INSERT INTO caches ("
                        "id, absolute_uri, request_headers, "
                        "response_headers, num_parts, date_created, "
                        "date_updated, status_code, status_description, size, access_value"
                      ") VALUES ("
                        ":id, :absolute_uri, :request_headers, "
                        ":response_headers, :num_parts, :date_created, "
                        ":date_updated, :status_code, :status_description, :size, :access_value"
                      ")");
    } else {
        accessCount = query.value(query.record().indexOf("access_count")).toInt();
        query.prepare("UPDATE caches SET "
                        "absolute_uri = :absolute_uri, request_headers = :request_headers, "
                        "response_headers = :response_headers, num_parts = :num_parts, "
                        "date_updated = :date_updated, status_code = :status_code, "
                        "status_description = :status_description, size = :size, access_value = :access_value"
                      " WHERE id = :id");
    }
    QString timestamp = QDateTime::currentDateTime().toString(Qt::ISODate);
    accessCount++;

    query.bindValue(":id", m_request->hashCode());
    query.bindValue(":absolute_uri", m_request->url());
    query.bindValue(":request_headers", m_request->requestHeaders().toString());
    query.bindValue(":response_headers", m_proxyDownload->inputObject()->responseHeaders().toString());
    query.bindValue(":num_parts", m_numParts);
    query.bindValue(":date_updated", timestamp);
    query.bindValue(":status_code", m_proxyDownload->inputObject()->httpStatusCode().toInt());
    query.bindValue(":status_description", QString(m_proxyDownload->inputObject()->httpStatusDescription()));
    query.bindValue(":size", m_sizeWritten);
    query.bindValue(":access_value", ProxyDownloads::instance()->gdsfClock()->getGDSFPriority(accessCount, m_sizeWritten));

    if (!update)
        query.bindValue(":date_created", timestamp);

//    if (!query.exec()) {
//        MessageHelper::debug(query.lastError().text());
//        return false;
//    }
    return true;
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
    for (int i = 0; i < m_numParts; ++i) {
        QFile *file = cacheFolder.cacheFile(m_request, i);
        if (file->exists())
            file->remove();
        delete file;
    }
}

/**
 * @brief Creates a new output file to be written to in cache.
 */
void ProxyCacheOutputWriter::createCacheFile()
{
    CacheFolder cacheFolder;
    m_cacheFile = cacheFolder.cacheFile(m_request, m_numParts, this);
    m_cacheFile->open(QIODevice::WriteOnly);
    m_numParts++;
    m_partSizeWritten = 0;
}
