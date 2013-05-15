#include "proxystaticreader.h"

#include "proxyrequest.h"
#include "messagehelper.h"
#include "applicationdatastorage.h"
#include "sockethandler.h"

#include <QFile>
#include <QDir>

ProxyStaticReader::ProxyStaticReader(SocketHandler *socketHandler, ProxyRequest *request, QObject *parent) :
    QObject(parent),
    m_socketHandler(socketHandler),
    m_request(request)
{
}

void ProxyStaticReader::read()
{
    ApplicationDataStorage appDataStorage;
    QDir dir = appDataStorage.appResourcesDirectory();
    QString filePath = m_request->staticResourcePath();
    QString path = dir.absoluteFilePath(filePath);
    QString contentType = m_request->requestContentType();
    QFileInfo fi(path);

    if (fi.isDir()) {
        dir.setPath(fi.absoluteFilePath());
        QStringList entries = dir.entryList(QStringList("index.*"));
        if (!entries.count()) {
            error();
            return;
        }
        fi.setFile(dir.absoluteFilePath(entries.first()));
        contentType = m_request->requestContentType("", fi.suffix());
    }
    QFile *file = new QFile(fi.absoluteFilePath());

    if (file->exists() && file->open(QIODevice::ReadOnly)) {
        m_socketHandler->writeStatusCodeAndDescription(200, "OK");
        QVariantMap responseHeaders;
        responseHeaders.insert("Content-type", contentType);
        responseHeaders.insert("Content-length", file->size());
        m_socketHandler->writeHeaders(responseHeaders);

        m_socketHandler->write(file->readAll());
    } else {
        error();
    }
}

void ProxyStaticReader::error()
{
    m_socketHandler->writeStatusCodeAndDescription(404, "NOT FOUND");
    QVariantMap responseHeaders;
    responseHeaders.insert("Content-length", 0);
    m_socketHandler->writeHeaders(responseHeaders);

    m_socketHandler->write("");
}
