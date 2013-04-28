#include "multipartformreader.h"

#include "iproxyconnection.h"
#include "icachefolder.h"
#include "sharedfilesmanager.h"
#include "idatabaseupdatequery.h"
#include "idatabasesettings.h"

#include <QFile>
#include <QStringList>
#include <QBuffer>
#include <QUrl>

SharedFilesManager::SharedFilesManager(const QString &tempFileName, IProxyConnection *proxyConnection, QObject *parent) :
    QObject(parent),
    m_tempFileName(tempFileName),
    m_proxyConnection(proxyConnection)
{
}

QString SharedFilesManager::saveFileToCache(const QString &inputName)
{
    m_proxyConnection->message(m_tempFileName);
    if (m_tempFileName.isEmpty())
        return QString();
    QFile tempFile(m_tempFileName);
    tempFile.open(QFile::ReadOnly);

    QByteArray firstLine = tempFile.readLine();

    QByteArray fileContent;
    QString fileName;
    QString contentType;
    QString url;

    /// Find the part with the file
    enum ReadState {
        LookingForBoundary,
        ReadingFile,
        ReadingHeaders
    };
    ReadState readState = LookingForBoundary;

    while (!tempFile.atEnd()) {
        QByteArray line = tempFile.readLine();
        bool breakWhile = false;

        switch (readState) {
        case LookingForBoundary:
            if (line.startsWith(firstLine))
                break;

            if (line.startsWith("Content-")) {
                QString lineStr = QString(line).toLower();
                if (lineStr.startsWith("content-disposition") && lineStr.contains(QString("name=\"%1\"")
                                                                                  .arg(inputName.toLower()))) {
                    lineStr = QString(line);
                    fileName = lineStr.split("filename=\"").last().split("\"").first();
                    if (fileName.isEmpty())
                        break;

                    readState = ReadingHeaders;
                }
            }
            break;

        case ReadingHeaders:
            if (line.startsWith("Content-")) {
                QString lineStr = QString(line).toLower();
                if (lineStr.startsWith("content-type")) {
                    QStringList split = QString(line).split(":");
                    split.takeFirst();
                    contentType = split.join(":").trimmed();
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

    if (readState == ReadingFile) {
        ICacheFolder *folder = m_proxyConnection->cacheFolder();
        url = createUrl(fileName);
        QFile *cacheFile = folder->cacheFile(url, 0, this);
        int size = fileContent.size();

        cacheFile->open(QFile::WriteOnly);

        cacheFile->write("HTTP/1.1 200 OK\r\n");
        cacheFile->write(QString("Content-Type: %1\r\n").arg(contentType).toUtf8());
        cacheFile->write(QString("Content-Length: %1\r\n").arg(size).toUtf8());
        cacheFile->write("\r\n");

        cacheFile->write(fileContent);
        cacheFile->flush();
        cacheFile->close();

        saveToDatabase(url, contentType, size);
    }

    return url;
}

QString SharedFilesManager::createUrl(const QString &fileName)
{
    return QString("http://%1.ownet.local/%2/%3")
            .arg(m_proxyConnection->databaseSettings(this)->clientId())
            .arg(m_proxyConnection->generateUniqueId())
            .arg(QString(QUrl::toPercentEncoding(fileName)));
}

void SharedFilesManager::saveToDatabase(const QString &url, const QString &contentType, qint64 size)
{
    IDatabaseUpdateQuery *updateQuery = m_proxyConnection->databaseUpdateQuery("shared_files");
    updateQuery->setColumnValue("url", url);
    updateQuery->setColumnValue("title", m_title);
    updateQuery->setColumnValue("description", m_description);
    updateQuery->setColumnValue("cache_id", m_proxyConnection->cacheId(url));
    updateQuery->setColumnValue("content_type", contentType);
    updateQuery->setUpdateDates(true);
    updateQuery->executeQuery();

    m_proxyConnection->saveToCache(url, 1, size, 10);
}
