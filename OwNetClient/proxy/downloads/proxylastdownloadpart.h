#ifndef PROXYLASTDOWNLOADPART_H
#define PROXYLASTDOWNLOADPART_H

#include "proxydownloadpart.h"

/**
 * @brief Represents the last part of the download.
 */
class ProxyLastDownloadPart : public ProxyDownloadPart
{
public:
    ProxyLastDownloadPart(QObject *parent = NULL, bool isError = false);

    bool isLast() { return true; }
    bool isError() { return m_isError; }

private:
    bool m_isError;
};

#endif // PROXYLASTDOWNLOADPART_H
