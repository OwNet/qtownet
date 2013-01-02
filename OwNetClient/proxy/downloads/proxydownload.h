#ifndef PROXYDOWNLOAD_H
#define PROXYDOWNLOAD_H

#include <QObject>
#include <QMutex>
#include <QMap>

#include "listofstringpairs.h"

class ProxyInputObject;
class ProxyRequest;
class ProxyHandlerSession;
class QIODevice;
class ProxyDownloadPart;
class ProxyCacheFileDownloadPart;

/**
 * @brief Used as an intermediary between the input source (web, cache, disk) and the output writer (socket, cache)
 * Input is always one, can have multiple output readers.
 */
class ProxyDownload : public QObject
{
    Q_OBJECT

public:
    ProxyDownload(ProxyRequest *request, ProxyHandlerSession *handlerSession, QObject *parent = 0);
    ~ProxyDownload();

    enum {
        FirstDownloadPartIndex = 0,
        FirstReaderId = 1
    };

    ProxyInputObject *inputObject() { return m_inputObject; }

    int registerReader();
    void deregisterReader(int readerId);
    int countRegisteredReaders();
    uint hashCode() { return m_hashCode; }

    bool shareDownload() { return m_shareDownload; }
    void startDownload();

    void close();

    ProxyDownloadPart *downloadPart(int readerId);
    void replaceDownloadParts(ProxyCacheFileDownloadPart *downloadPart, int at);
    
signals:
    void bytePartAvailable();
    void downloadFinished();

public slots:

private slots:
    void readReply(QIODevice *ioDevice);
    void inputObjectFinished();
    void inputObjectError();
    
private:
    void deleteUnnecessaryParts();

    ProxyInputObject *webInputObject(ProxyRequest *request);

    ProxyInputObject *m_inputObject;
    ProxyHandlerSession *m_proxyHandlerSession;

    QMutex m_downloadPartsMutex;
    QMap<int, ProxyDownloadPart *> m_downloadParts;
    int m_nextDownloadPartIndex;

    QMutex m_readersMutex;
    QMap<int, int> m_readers;
    int m_nextReaderId;
    uint m_hashCode;
    int m_proxyHandlerDependentObjectId;

    bool m_shareDownload;
};

#endif // PROXYDOWNLOAD_H
