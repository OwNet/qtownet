#ifndef PROXYCACHEOUTPUTWRITER_H
#define PROXYCACHEOUTPUTWRITER_H

#include "proxyoutputwriter.h"

class QFile;

class ProxyCacheOutputWriter : public ProxyOutputWriter
{
public:
    ProxyCacheOutputWriter(ProxyDownload *download, ProxyHandler *proxyHandler, QObject *parent = NULL);

    enum {
        MaxFileSize = 524288 // 0.5 MB
    };

    void finish();

protected:
    void close();
    void read(QIODevice *ioDevice);
    bool save();

private:
    void createCacheFile();

    QFile *m_cacheFile;
    ProxyRequest *m_request;
    long m_partSizeWritten;
    int m_numParts;
};

#endif // PROXYCACHEOUTPUTWRITER_H
