#ifndef PROXYDOWNLOAD_H
#define PROXYDOWNLOAD_H

#include <QObject>
#include <QMutex>
#include <QMap>

#include "listofstringpairs.h"

class ProxyRequest;
class ProxyInputObject;
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

    int registerReader();
    void deregisterReader(int readerId);
    int countRegisteredReaders();
    uint hashCode() { return m_hashCode; }

    bool shareDownload();
    bool headersInBody() const { return m_headersInBody; }

    ProxyDownloadPart *downloadPart(int readerId);
    void replaceDownloadParts(ProxyDownloadPart *downloadPart, int at);

    ProxyInputObject *inputObject() const { return m_inputObject; }
    void setInputObject(ProxyInputObject *inputObject);
    
signals:
    void bytePartAvailable();
    void downloadFinished();

public slots:
    void readReply(QIODevice *ioDevice);
    void inputObjectFinished();
    void inputObjectError();
    
private:
    void deleteUnnecessaryParts();

    ProxyHandlerSession *m_proxyHandlerSession;
    ProxyInputObject *m_inputObject;

    QMutex m_downloadPartsMutex;
    QMap<int, ProxyDownloadPart *> m_downloadParts;
    int m_nextDownloadPartIndex;

    QMutex m_readersMutex;
    QMap<int, int> m_readers;
    int m_nextReaderId;
    uint m_hashCode;

    bool m_headersInBody;
};

#endif // PROXYDOWNLOAD_H
