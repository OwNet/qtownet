#include "proxycacheoutputwriter.h"

#include "proxydownload.h"
#include "cachefolder.h"
#include "proxyinputobject.h"
#include "proxyrequest.h"
#include "messagehelper.h"

#include <QFile>
#include <QIODevice>
#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>

ProxyCacheOutputWriter::ProxyCacheOutputWriter(ProxyDownload *download, ProxyHandler *proxyHandler, QObject *parent)
    : ProxyOutputWriter(proxyHandler, parent), m_partSizeWritten(0), m_numParts(0)
{
    m_proxyDownload = download;
    m_request = m_proxyDownload->inputObject()->request();
    createCacheFile();
    connectProxyDownload();
    m_downloadReaderId = m_proxyDownload->registerReader();
}

void ProxyCacheOutputWriter::finish()
{
    close();
}

void ProxyCacheOutputWriter::close()
{
    if (m_cacheFile)
        m_cacheFile->close();

    save();
    ProxyOutputWriter::close();
}

void ProxyCacheOutputWriter::read(QIODevice *ioDevice)
{
    m_cacheFile->write(ioDevice->readAll());
    m_partSizeWritten += ioDevice->size();

    if (m_partSizeWritten > MaxFileSize) {
        m_cacheFile->close();
        delete m_cacheFile;

        createCacheFile();
    }
}

bool ProxyCacheOutputWriter::save()
{
    bool update = false;

    QSqlQuery query;
    query.prepare("SELECT 1 FROM caches WHERE id = :id LIMIT 1");
    query.bindValue(":id", m_proxyDownload->hashCode());
    update = query.exec() && query.next();

    if (!update) {
        query.prepare("INSERT INTO caches ("
                        "id, absolute_uri, request_headers, response_headers, num_parts, date_created, date_updated"
                      ") VALUES ("
                        ":id, :absolute_uri, :request_headers, :response_headers, :num_parts, :date_created, :date_updated"
                      ")");
    } else {
        query.prepare("UPDATE caches SET"
                        " absolute_uri = :absolute_uri, request_headers = :request_headers, response_headers = :response_headers, num_parts = :num_parts, date_updated = :date_updated"
                      " WHERE id = :id");
    }
    QString timestamp = QDateTime::currentDateTime().toString(Qt::ISODate);

    query.bindValue(":id", m_request->hashCode());
    query.bindValue(":absolute_uri", m_request->url());
    query.bindValue(":request_headers", m_request->requestHeaders().toString());
    query.bindValue(":response_headers", m_proxyDownload->inputObject()->responseHeaders().toString());
    query.bindValue(":num_parts", m_numParts);
    query.bindValue(":date_updated", timestamp);

    if (!update)
        query.bindValue(":date_created", timestamp);

    if (!query.exec()) {
        MessageHelper::debug(query.lastError().text());
        return false;
    }
    return true;
}

void ProxyCacheOutputWriter::createCacheFile()
{
    CacheFolder cacheFolder;
    m_cacheFile = cacheFolder.cacheFile(m_request, m_numParts);
    m_cacheFile->open(QIODevice::WriteOnly);
    m_numParts++;
    m_partSizeWritten = 0;
}
