#include "proxycachefiledownloadpart.h"

#include "proxydownloadstream.h"

#include <QFile>
#include <QBuffer>

ProxyCacheFileDownloadPart::ProxyCacheFileDownloadPart(const QString &path, int nextDownloadPartIndex, QObject *parent)
    : ProxyDownloadPart(nextDownloadPartIndex, parent), m_path(path)
{
}

ProxyDownloadStream *ProxyCacheFileDownloadPart::stream()
{
    ProxyDownloadStream *s = new ProxyDownloadStream;
    QFile *file = new QFile(m_path, s);
    if (file->open(QIODevice::ReadOnly))
        s->setStream(file);
    return s;
}
