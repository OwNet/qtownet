#include "proxycacheinputobject.h"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QFile>

#include "proxyrequest.h"
#include "cachefolder.h"
#include "qjson/json_parser.hh"
#include "gdsfclock.h"
#include "proxydownloads.h"

ProxyCacheInputObject::ProxyCacheInputObject(ProxyRequest *request, QObject *parent)
    : ProxyInputObject(request, parent), m_exists(false), m_numParts(0)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM caches WHERE id = :id");
    query.bindValue(":id", request->hashCode());
    if (query.exec() && query.next()) {
        m_exists = true;
        m_numParts = query.value(query.record().indexOf("num_parts")).toInt();

        setHttpStatusCode(query.value(query.record().indexOf("status_code")).toInt());
        setHttpStatusDescription(query.value(query.record().indexOf("status_description")).toString());

        bool ok;
        QJson::Parser parser;
        QVariantMap result = parser.parse(query.value(query.record().indexOf("response_headers"))
                                          .toByteArray(), &ok)
                .toMap();
        if (ok)
            m_responseHeaders = VariantMap(result);

        int accessCount = query.value(query.record().indexOf("access_count")).toInt() + 1;
        long size = query.value(query.record().indexOf("size")).toLongLong();

        query.prepare("UPDATE caches SET access_count = :access_count, access_value = :access_value "
                      "WHERE id = :id");
        query.bindValue(":id", request->hashCode());
        query.bindValue(":access_count", accessCount);
        query.bindValue(":access_value", ProxyDownloads::instance()->gdsfClock()->getGDSFPriority(accessCount, size));
        query.exec();
    }
}

void ProxyCacheInputObject::readRequest()
{
    if (!exists()) {
        emit failed();
        return;
    }

    CacheFolder cacheFolder;
    for (int i = 0; i < m_numParts; ++i) {
        QFile *file = cacheFolder.cacheFile(m_request, i);
        if (!file->exists()) {
            delete file;
            break;
        }

        if (file->open(QIODevice::ReadOnly))
            emit readyRead(file);
    }

    emit finished();
}