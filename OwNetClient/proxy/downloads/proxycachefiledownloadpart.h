#ifndef PROXYCACHEFILEDOWNLOADPART_H
#define PROXYCACHEFILEDOWNLOADPART_H

#include "proxydownloadpart.h"

class ProxyCacheFileDownloadPart : public ProxyDownloadPart
{
public:
    ProxyCacheFileDownloadPart(const QString &path, int nextDownloadPartIndex, QObject *parent = NULL);

    QIODevice *stream();

private:
    QString m_path;
};

#endif // PROXYCACHEFILEDOWNLOADPART_H
