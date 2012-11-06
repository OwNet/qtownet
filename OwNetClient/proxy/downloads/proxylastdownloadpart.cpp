#include "proxylastdownloadpart.h"

ProxyLastDownloadPart::ProxyLastDownloadPart(QObject *parent, bool isError)
    : ProxyDownloadPart(parent), m_isError(isError)
{
}
