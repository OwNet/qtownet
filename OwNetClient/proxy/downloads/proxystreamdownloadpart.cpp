#include "proxystreamdownloadpart.h"

#include "proxydownloadstream.h"

#include <QIODevice>

ProxyStreamDownloadPart::ProxyStreamDownloadPart(QIODevice *stream, int nextDownloadPartIndex, QObject *parent)
    : ProxyDownloadPart(nextDownloadPartIndex, parent), m_stream(stream)
{
}

ProxyStreamDownloadPart::~ProxyStreamDownloadPart()
{
    if (m_stream->isOpen())
        m_stream->close();
    delete m_stream;
}

ProxyDownloadStream *ProxyStreamDownloadPart::stream()
{
    ProxyDownloadStream *s = new ProxyDownloadStream;
    s->setStream(m_stream);
    return s;
}
