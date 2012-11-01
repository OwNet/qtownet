#ifndef PROXYDOWNLOAD_H
#define PROXYDOWNLOAD_H

#include <QObject>
#include <QMutex>
#include <QMap>
#include <QQueue>

#include "listofstringpairs.h"

class ProxyInputObject;
class ProxyRequest;
class QIODevice;

class ProxyDownload : public QObject
{
    Q_OBJECT
public:
    ProxyDownload(ProxyRequest *request, QObject *parent = 0);

    ProxyInputObject *inputObject() { return m_inputObject; }

    int registerReader();
    void deregisterReader(int readerId);
    bool shareDownload() { return m_shareDownload; }
    void startDownload();

    QIODevice *bytePart(int readerId);
    
signals:
    void bytePartAvailable();
    void downloadFinished();

public slots:

private slots:
    void readReply(QIODevice *ioDevice);
    void inputObjectFinished();
    
private:
    ProxyInputObject *m_inputObject;

    QMutex m_bytePartsMutex;
    QList<QByteArray *> m_byteParts;
    QQueue<QIODevice *> m_availableStreamsQueue;

    QMutex m_readersMutex;
    QMap<int, int> m_readers;
    int m_nextReaderId;

    bool m_shareDownload;
};

#endif // PROXYDOWNLOAD_H
