#include "sharedfilesmanager.h"

#include "iproxyconnection.h"
#include "icachefolder.h"
#include "sharedfilesmanager.h"
#include "idatabaseupdatequery.h"
#include "idatabasesettings.h"

#include <QFile>
#include <QStringList>
#include <QBuffer>
#include <QUrl>

SharedFilesManager::SharedFilesManager(IProxyConnection *proxyConnection, QObject *parent) :
    QObject(parent),
    m_proxyConnection(proxyConnection)
{
}

SharedFilesManager::SharedFilesManager(const QString &tempFileName, IProxyConnection *proxyConnection, QObject *parent) :
    QObject(parent),
    m_tempFileName(tempFileName),
    m_proxyConnection(proxyConnection)
{
}

void SharedFilesManager::saveFileToCache()
{
    m_proxyConnection->message(m_tempFileName);
    if (m_tempFileName.isEmpty())
        return;

    QFile tempFile(m_tempFileName);
    tempFile.open(QFile::ReadOnly);

    QByteArray fileContent = getValueFor(&tempFile, "selectfile", true);
    m_title = QString(getValueFor(&tempFile, "title", false).trimmed());
    m_description = QString(getValueFor(&tempFile, "description", false).trimmed());

    if (!fileContent.isEmpty()) {
        ICacheFolder *folder = m_proxyConnection->cacheFolder();
        QString url = createUrl(m_fileName);
        QFile *cacheFile = folder->cacheFile(url, 0, this);
        int size = fileContent.size();

        cacheFile->open(QFile::WriteOnly);

        cacheFile->write("HTTP/1.1 200 OK\r\n");
        cacheFile->write(QString("Content-Type: %1\r\n").arg(m_contentType).toUtf8());
        cacheFile->write(QString("Content-Length: %1\r\n").arg(size).toUtf8());
        cacheFile->write("\r\n");

        cacheFile->write(fileContent);
        cacheFile->flush();
        cacheFile->close();

        saveToDatabase(url, size);
    }
}

QVariantList SharedFilesManager::listAvailableFiles()
{
    QObject parent;
    IDatabaseSelectQuery *query = m_proxyConnection->databaseSelect("shared_files", &parent);
    query->orderBy("title");

    QVariantList results;

    while (query->next()) {
        if (!m_proxyConnection->isCacheAvailable(query->value("cache_id").toUInt()))
            continue;

        QVariantMap info;
        info.insert("uid", query->value("uid").toString());
        info.insert("title", query->value("title").toString());
        info.insert("description", query->value("description").toString());
        info.insert("url", query->value("url").toString());
        info.insert("date_created", query->value("date_created").toString());
        info.insert("content_type", query->value("content_type").toString());
        results.append(info);
    }

    return results;
}

void SharedFilesManager::removeFile(const QString &uid)
{
    IDatabaseUpdateQuery *query = m_proxyConnection->databaseUpdateQuery("shared_files", this);
    query->setType(IDatabaseUpdateQuery::Delete);
    query->singleWhere("uid", uid);
    query->executeQuery();
}

QByteArray SharedFilesManager::getValueFor(QFile *tempFile, const QString &key, bool findFileName)
{
    tempFile->seek(0);
    QByteArray firstLine = tempFile->readLine();
    QByteArray fileContent;

    enum ReadState {
        LookingForBoundary,
        ReadingFile,
        ReadingHeaders
    };
    ReadState readState = LookingForBoundary;

    while (!tempFile->atEnd()) {
        QByteArray line = tempFile->readLine();
        bool breakWhile = false;

        switch (readState) {
        case LookingForBoundary:
            if (line.startsWith(firstLine))
                break;

            if (line.startsWith("Content-")) {
                QString lineStr = QString(line).toLower();
                if (lineStr.startsWith("content-disposition") && lineStr.contains(QString("name=\"%1\"")
                                                                                  .arg(key.toLower()))) {
                    if (findFileName) {
                        lineStr = QString(line);
                        m_fileName = lineStr.split("filename=\"").last().split("\"").first();
                        if (m_fileName.isEmpty())
                            break;
                    }

                    readState = ReadingHeaders;
                }
            }
            break;

        case ReadingHeaders:
            if (findFileName && line.startsWith("Content-")) {
                QString lineStr = QString(line).toLower();
                if (lineStr.startsWith("content-type")) {
                    QStringList split = QString(line).split(":");
                    split.takeFirst();
                    m_contentType = split.join(":").trimmed();
                }
            } else if (line.trimmed().isEmpty()) {
                readState = ReadingFile;
            }
            break;

        case ReadingFile:
            if (line.startsWith(firstLine)) {
                breakWhile = true;
                break;
            }

            fileContent.append(line);
            break;
        }

        if (breakWhile)
            break;
    }

    if (readState == ReadingFile)
        return fileContent;
    return QByteArray();
}

QString SharedFilesManager::createUrl(const QString &fileName)
{
    return QString("http://%1.ownet.local/%2/%3")
            .arg(m_proxyConnection->databaseSettings(this)->clientId())
            .arg(m_proxyConnection->generateUniqueId())
            .arg(QString(QUrl::toPercentEncoding(fileName)));
}

void SharedFilesManager::saveToDatabase(const QString &url, qint64 size)
{
    m_proxyConnection->saveToCache(url, 1, size, 10);

    IDatabaseUpdateQuery *updateQuery = m_proxyConnection->databaseUpdateQuery("shared_files");
    updateQuery->setColumnValue("url", url);
    updateQuery->setColumnValue("title", m_title);
    updateQuery->setColumnValue("description", m_description);
    updateQuery->setColumnValue("cache_id", m_proxyConnection->cacheId(url));
    updateQuery->setColumnValue("content_type", m_contentType);
    updateQuery->setUpdateDates(true);
    updateQuery->executeQuery();
}
