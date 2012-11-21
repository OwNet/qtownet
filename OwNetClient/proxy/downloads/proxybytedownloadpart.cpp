#include "proxybytedownloadpart.h"

#include <QByteArray>
#include <QBuffer>

ProxyByteDownloadPart::ProxyByteDownloadPart(QByteArray *bytes, int nextDownloadPartIndex, QObject *parent)
    : ProxyDownloadPart(nextDownloadPartIndex, parent), m_bytes(bytes)
{
}

ProxyByteDownloadPart::~ProxyByteDownloadPart()
{
    delete m_bytes;
}

QIODevice *ProxyByteDownloadPart::stream()
{
    QBuffer *buffer = new QBuffer(m_bytes);
    buffer->open(QIODevice::ReadOnly);

    return buffer;
}
