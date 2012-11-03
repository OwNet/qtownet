#ifndef PROXYOUTPUTWRITER_H
#define PROXYOUTPUTWRITER_H

#include <QObject>
#include <QMutex>

class ProxyRequest;
class ProxyDownload;
class ProxyInputObject;
class ProxyDownloads;
class QIODevice;
class ProxyHandler;

/**
 * @brief Reads data from ProxyDownload and outputs it to a custom source
 */
class ProxyOutputWriter : public QObject
{
    Q_OBJECT
public:
    explicit ProxyOutputWriter(ProxyHandler *proxyHandler, QObject *parent = NULL);

    virtual void finish() = 0;

protected:
    void createDownload(ProxyRequest *request);

    virtual void close();
    virtual void read(QIODevice *ioDevice) = 0;

    ProxyDownload *m_proxyDownload;

signals:
    void iAmActive();
    
private slots:
    void readAvailableParts();
    void downloadFinished();

protected:
    void connectProxyDownload();

    ProxyHandler *m_proxyHandler;
    ProxyDownloads *m_proxyDownloads;
    int m_downloadReaderId;

    QMutex m_readingMutex;

    int m_lastPartRead;
};

#endif // PROXYOUTPUTWRITER_H
