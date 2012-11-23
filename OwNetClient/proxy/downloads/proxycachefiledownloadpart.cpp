#include "proxycachefiledownloadpart.h"

#include <QFile>
#include <QBuffer>

ProxyCacheFileDownloadPart::ProxyCacheFileDownloadPart(const QString &path, int nextDownloadPartIndex, QObject *parent)
    : ProxyDownloadPart(nextDownloadPartIndex, parent), m_path(path)
{
}

QIODevice *ProxyCacheFileDownloadPart::stream()
{
    QFile *file = new QFile(m_path, this);
    if (file->open(QIODevice::ReadOnly))
        return file;
    return new QBuffer(this);
}
