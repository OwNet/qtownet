#ifndef PROXYOUTPUTWRITER_H
#define PROXYOUTPUTWRITER_H

#include <QObject>
#include <QMutex>

class ProxyRequest;
class ProxyDownload;
class ProxyInputObject;
class ProxyDownloads;
class QIODevice;
class ProxyHandlerSession;

/**
 * @brief Reads data from ProxyDownload and outputs it to a custom source
 */
class ProxyOutputWriter : public QObject
{
    Q_OBJECT
public:
    explicit ProxyOutputWriter(ProxyHandlerSession *proxyHandlerSession);

signals:
    void iAmActive();
    
public slots:
    void readAvailableParts();

protected slots:
    void forceQuit();

protected:
    void createDownload(ProxyRequest *request);

    virtual void virtualClose() = 0;
    virtual void read(QIODevice *ioDevice) = 0;
    virtual void error() = 0;

    ProxyHandlerSession *m_proxyHandlerSession;
    ProxyDownload *m_proxyDownload;
    ProxyDownloads *m_proxyDownloads;
    int m_downloadReaderId;

private:
    void close();

    QMutex m_readingMutex;
    int m_lastPartRead;
    bool m_closed;
    int m_proxyHandlerDependentObjectId;
};

#endif // PROXYOUTPUTWRITER_H
