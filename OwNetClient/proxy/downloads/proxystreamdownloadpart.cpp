#include "proxystreamdownloadpart.h"

#include <QIODevice>

ProxyStreamDownloadPart::ProxyStreamDownloadPart(QIODevice *stream, QObject *parent)
    : ProxyDownloadPart(parent), m_stream(stream)
{
}

ProxyStreamDownloadPart::~ProxyStreamDownloadPart()
{
    if (m_stream->isOpen())
        m_stream->close();
    delete m_stream;
}
