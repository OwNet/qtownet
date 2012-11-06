#ifndef PROXYDOWNLOAD_H
#define PROXYDOWNLOAD_H

#include <QObject>
#include <QMutex>
#include <QMap>

#include "listofstringpairs.h"

class ProxyInputObject;
class ProxyRequest;
class ProxyHandler;
class QIODevice;
class ProxyDownloadPart;

/**
 * @brief Used as an intermediary between the input source (web, cache, disk) and the output writer (socket, cache)
 * Input is always one, can have multiple output readers.
 */
class ProxyDownload : public QObject
{
    Q_OBJECT
public:
    ProxyDownload(ProxyRequest *request, ProxyHandler *handler, QObject *parent = 0);

    ProxyInputObject *inputObject() { return m_inputObject; }

    int registerReader();
    void deregisterReader(int readerId);
    int countRegisteredReaders();
    int hashCode() { return m_hashCode; }

    bool shareDownload() { return m_shareDownload; }
    void startDownload();

    void close();

    ProxyDownloadPart *downloadPart(int readerId);
    
signals:
    void bytePartAvailable();
    void downloadFinished();

public slots:

private slots:
    void readReply(QIODevice *ioDevice);
    void inputObjectFinished();
    void inputObjectError();
    
private:
    ProxyInputObject *webInputObject(ProxyRequest *request);

    ProxyInputObject *m_inputObject;
    ProxyHandler *m_proxyHandler;

    QMutex m_downloadPartsMutex;
    QList<ProxyDownloadPart *> m_downloadParts;

    QMutex m_readersMutex;
    QMap<int, int> m_readers;
    int m_nextReaderId;
    int m_hashCode;
    int m_proxyHandlerDependentObjectId;

    bool m_shareDownload;
};

#endif // PROXYDOWNLOAD_H
