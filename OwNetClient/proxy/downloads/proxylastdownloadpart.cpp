#include "proxylastdownloadpart.h"

ProxyLastDownloadPart::ProxyLastDownloadPart(QObject *parent, bool isError)
    : ProxyDownloadPart(-1, parent), m_isError(isError)
{
}
