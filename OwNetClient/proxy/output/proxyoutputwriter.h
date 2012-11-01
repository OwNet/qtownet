#ifndef PROXYOUTPUTWRITER_H
#define PROXYOUTPUTWRITER_H

#include <QObject>
#include <QMutex>

class ProxyRequest;
class ProxyDownload;
class ProxyInputObject;
class ProxyDownloads;
class QIODevice;

/**
 * @brief Reads data from ProxyDownload and outputs it to a custom source
 */
class ProxyOutputWriter : public QObject
{
    Q_OBJECT
public:
    explicit ProxyOutputWriter(QObject *parent = 0);

    virtual void finish() = 0;

protected:
    void createDownload(ProxyRequest *request);
    void registerDownload(ProxyDownload *download);

    virtual void close();
    virtual void read(QIODevice *ioDevice) = 0;

    ProxyDownload *m_proxyDownload;

signals:
    void iAmActive();
    
private slots:
    void readAvailableParts();
    void downloadFinished();
    
private:
    void registerDownload();

    ProxyDownloads *m_proxyDownloads;
    int m_downloadReaderId;

    QMutex m_readingMutex;

    int m_lastPartRead;
};

#endif // PROXYOUTPUTWRITER_H
