#include "proxybytedownloadpart.h"

#include "proxydownloadstream.h"

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

ProxyDownloadStream *ProxyByteDownloadPart::stream()
{
    ProxyDownloadStream *s = new ProxyDownloadStream;
    QBuffer *buffer = new QBuffer(m_bytes, s);
    buffer->open(QIODevice::ReadOnly);
    s->setStream(buffer);

    return s;
}
