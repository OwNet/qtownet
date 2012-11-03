#include "proxycacheinputobject.h"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QFile>

#include "proxyrequest.h"
#include "cachefolder.h"
#include "qjson/json_parser.hh"

ProxyCacheInputObject::ProxyCacheInputObject(ProxyRequest *request, QObject *parent)
    : ProxyInputObject(request, parent), m_exists(false), m_numParts(0)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM caches WHERE id = :id");
    query.bindValue(":id", request->hashCode());
    if (query.exec()) {
        if (query.next()) {
            m_exists = true;
            m_numParts = query.value(query.record().indexOf("num_parts")).toInt();

            bool ok;
            QJson::Parser parser;
            QVariantMap result = parser.parse(query.value(query.record().indexOf("response_headers"))
                                              .toByteArray(), &ok)
                    .toMap();
            if (ok)
                m_responseHeaders.parse(result);
        }
    }
}

void ProxyCacheInputObject::readRequest()
{
    if (!exists()) {
        emit finished();
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
