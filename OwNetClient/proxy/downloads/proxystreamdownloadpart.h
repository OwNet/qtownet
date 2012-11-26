#ifndef PROXYSTREAMDOWNLOADPART_H
#define PROXYSTREAMDOWNLOADPART_H

#include "proxydownloadpart.h"

/**
 * @brief A download part that contains data as a stream, downloaded when accessed.
 * Can't be read by multiple output writers.
 */
class ProxyStreamDownloadPart : public ProxyDownloadPart
{
public:
    ProxyStreamDownloadPart(QIODevice *stream, int nextDownloadPartIndex, QObject *parent = NULL);
    ~ProxyStreamDownloadPart();

    QIODevice *stream() { return m_stream; }

private:
    QIODevice *m_stream;
};

#endif // PROXYSTREAMDOWNLOADPART_H
