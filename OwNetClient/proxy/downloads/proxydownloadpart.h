#ifndef PROXYDOWNLOADPART_H
#define PROXYDOWNLOADPART_H

class QIODevice;

#include <QObject>

/**
 * @brief Part of the response created and stored in the ProxyDownload object.
 * This class is abstract, subclassed for specific stream types.
 */
class ProxyDownloadPart : public QObject
{
    Q_OBJECT

public:
    explicit ProxyDownloadPart(QObject *parent = 0);

    virtual bool isLast() { return false; }
    virtual bool isError() { return false; }
    virtual QIODevice *stream() { return NULL; }
};

#endif // PROXYDOWNLOADPART_H
