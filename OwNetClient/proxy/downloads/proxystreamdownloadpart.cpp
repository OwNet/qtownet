#include "proxystreamdownloadpart.h"

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
