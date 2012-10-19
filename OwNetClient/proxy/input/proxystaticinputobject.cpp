#include "proxystaticinputobject.h"

#include "proxyrequest.h"
#include "messagehelper.h"

#include <QFile>
#include <QDir>

ProxyStaticInputObject::ProxyStaticInputObject(ProxyRequest *request, QObject *parent)
    : ProxyInputObject(request, parent)
{
    m_responseHeaders.insert("Content-type", m_request->requestContentType());
}

void ProxyStaticInputObject::readRequest()
{
    MessageHelper::debug(m_request->url());

    QFile *file = new QFile(QString("OwNetClient/static/%1").arg(m_request->relativeUrl()));

    if (file->exists()) {
        if (file->open(QIODevice::ReadOnly))
            emit readyRead(file, this);

        file->close();
    } else {
        MessageHelper::debug(QString("404 NOT FOUND static/%1").arg(m_request->relativeUrl()));
    }

    emit finished();
    delete file;
}