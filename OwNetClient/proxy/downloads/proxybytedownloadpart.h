#ifndef PROXYBYTEDOWNLOADPART_H
#define PROXYBYTEDOWNLOADPART_H

#include "proxydownloadpart.h"

/**
 * @brief A download part that contains data in bytes.
 * Can be read by multiple output writers.
 */
class ProxyByteDownloadPart : public ProxyDownloadPart
{
public:
    ProxyByteDownloadPart(QByteArray *bytes, QObject *parent = NULL);
    ~ProxyByteDownloadPart();

    QIODevice *stream();

private:
    QByteArray *m_bytes;
};

#endif // PROXYBYTEDOWNLOADPART_H
