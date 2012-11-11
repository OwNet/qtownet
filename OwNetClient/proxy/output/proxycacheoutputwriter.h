#ifndef PROXYCACHEOUTPUTWRITER_H
#define PROXYCACHEOUTPUTWRITER_H

#include "proxyoutputwriter.h"

class QFile;

/**
 * @brief Reads data from ProxyDownload and outputs them to cache.
 * Data in cache can be written in multiple files with a specified size.
 */
class ProxyCacheOutputWriter : public ProxyOutputWriter
{
public:
    ProxyCacheOutputWriter(ProxyDownload *download, ProxyHandler *proxyHandler, QObject *parent = NULL);

    enum {
        MaxFileSize = 524288 // 0.5 MB
    };

protected:
    void virtualClose();
    void read(QIODevice *ioDevice);
    bool save();
    void error();

private:
    void createCacheFile();

    QFile *m_cacheFile;
    ProxyRequest *m_request;
    long m_partSizeWritten;
    int m_numParts;
    bool m_failed;
};

#endif // PROXYCACHEOUTPUTWRITER_H
