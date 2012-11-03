#ifndef PROXYDOWNLOAD_H
#define PROXYDOWNLOAD_H

#include <QObject>
#include <QMutex>
#include <QMap>
#include <QQueue>

#include "listofstringpairs.h"

class ProxyInputObject;
class ProxyRequest;
class ProxyHandler;
class QIODevice;

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
    bool reuseBuffers() { return m_reuseBuffers; }
    void startDownload();

    void close();

    QIODevice *bytePart(int readerId);
    
signals:
    void bytePartAvailable();
    void downloadFinished();

public slots:

private slots:
    void readReply(QIODevice *ioDevice);
    void inputObjectFinished();
    
private:
    ProxyInputObject *webInputObject(ProxyRequest *request);

    ProxyInputObject *m_inputObject;
    ProxyHandler *m_proxyHandler;

    QMutex m_bytePartsMutex;
    QList<QByteArray *> m_byteParts;
    QQueue<QIODevice *> m_availableStreamsQueue;

    QMutex m_readersMutex;
    QMap<int, int> m_readers;
    int m_nextReaderId;
    int m_hashCode;
    int m_proxyHandlerDependentObjectId;

    bool m_shareDownload;
    bool m_reuseBuffers;
};

#endif // PROXYDOWNLOAD_H
