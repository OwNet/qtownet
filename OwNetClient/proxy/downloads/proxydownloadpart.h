#ifndef PROXYDOWNLOADPART_H
#define PROXYDOWNLOADPART_H

class QIODevice;
class ProxyDownloadStream;

#include <QObject>
#include <QSemaphore>

/**
 * @brief Part of the response created and stored in the ProxyDownload object.
 * This class is abstract, subclassed for specific stream types.
 */
class ProxyDownloadPart : public QObject
{
    Q_OBJECT

public:
    explicit ProxyDownloadPart(int nextDownloadPartIndex, QObject *parent = 0);

    virtual bool isLast() { return false; }
    virtual bool isError() { return false; }
    virtual ProxyDownloadStream *stream() { return NULL; }

    int nextDownloadPartIndex() const { return m_nextDownloadPartIndex; }

    void deletePreviousPartAfterReadersRegister(ProxyDownloadPart *previousPart, QList<int> readerIds);
    void registerReader(int readerId);

signals:
    void destroySelf();

private:
    void setParentPart(ProxyDownloadPart *parentPart);

    int m_nextDownloadPartIndex;
    QList<int> m_readersOfPreviousParts;
    ProxyDownloadPart *m_previousPart;
};

#endif // PROXYDOWNLOADPART_H
