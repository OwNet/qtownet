#include "proxydownloadstream.h"

ProxyDownloadStream::ProxyDownloadStream(QObject *parent) :
    QObject(parent),
    m_stream(NULL)
{
}
