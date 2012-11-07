#include "proxystaticinputobject.h"

#include "proxyrequest.h"
#include "messagehelper.h"
#include "applicationdatastorage.h"

#include <QFile>
#include <QDir>
#include <QApplication>

ProxyStaticInputObject::ProxyStaticInputObject(ProxyRequest *request, QObject *parent)
    : ProxyInputObject(request, parent)
{
    m_httpStatusCode = QString::number(200);
    m_httpStatusDescription = "OK";

    addHeader("Content-type", m_request->requestContentType());
}

void ProxyStaticInputObject::readRequest()
{
    ApplicationDataStorage appDataStorage;
    QDir dir = appDataStorage.appDataDirectory();
    QFile *file = new QFile(dir.absoluteFilePath(QString("static/%1")
                                                 .arg(m_request->relativeUrl())));

    if (file->exists()) {
        if (file->open(QIODevice::ReadOnly))
            emit readyRead(file);

        emit finished();
    } else {
        MessageHelper::debug(QString("404 NOT FOUND static/%1").arg(m_request->relativeUrl()));
        emit failed();
    }
}
