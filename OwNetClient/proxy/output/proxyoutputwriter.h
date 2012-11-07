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

    void finish();

signals:
    void iAmActive();
    void finished();
    
private slots:
    void readAvailableParts();

protected:
    void connectProxyDownload();
    void createDownload(ProxyRequest *request);

    virtual void virtualClose() = 0;
    virtual void read(QIODevice *ioDevice) = 0;
    virtual void error() = 0;

    ProxyHandler *m_proxyHandler;
    ProxyDownload *m_proxyDownload;
    ProxyDownloads *m_proxyDownloads;
    int m_downloadReaderId;

private:
    void close();

    QMutex m_readingMutex;
    int m_lastPartRead;
    bool m_closed;
};

#endif // PROXYOUTPUTWRITER_H
