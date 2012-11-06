#include "proxydownload.h"

#include "proxywebinputobject.h"
#include "proxystaticinputobject.h"
#include "proxyrequestbus.h"
#include "proxyrequest.h"
#include "proxyhandler.h"

#include <QBuffer>
#include <QDebug>

ProxyDownload::ProxyDownload(ProxyRequest *request, ProxyHandler *handler, QObject *parent) :
    QObject(parent), m_inputObject(NULL), m_proxyHandler(handler), m_nextReaderId(1), m_shareDownload(false), m_reuseBuffers(true)
{
    m_hashCode = request->hashCode();
    m_proxyHandlerDependentObjectId = m_proxyHandler->registerDependentObject();

    if (request->isStaticResourceRequest()) {
        m_inputObject = new ProxyStaticInputObject(request, this);
    } else if (request->isLocalRequest()) {
        m_inputObject = new ProxyRequestBus(request, this);
    } else {
        m_inputObject = new ProxyWebInputObject(request, this);
        m_shareDownload = true;
        //m_reuseBuffers = true;
    }

    connect(m_inputObject, SIGNAL(finished()), this, SLOT(inputObjectFinished()));
    connect(m_inputObject, SIGNAL(readyRead(QIODevice*)), this, SLOT(readReply(QIODevice*)));
}

int ProxyDownload::registerReader()
{
    int readerId = -1;

    m_readersMutex.lock();
    readerId = m_nextReaderId;
    m_nextReaderId++;
    m_readers.insert(readerId, 0);
    m_readersMutex.unlock();

    return readerId;
}

void ProxyDownload::deregisterReader(int readerId)
{
    m_readersMutex.lock();
    m_readers.remove(readerId);
    m_readersMutex.unlock();
}

int ProxyDownload::countRegisteredReaders()
{
    QMutexLocker mutexLocker(&m_readersMutex);

    return m_readers.count();
}

void ProxyDownload::startDownload()
{
    m_inputObject->startDownload();
}

void ProxyDownload::close()
{
    m_proxyHandler->deregisterDependentObject(m_proxyHandlerDependentObjectId);
}

QIODevice *ProxyDownload::bytePart(int readerId)
{
    if (m_shareDownload) {
        QByteArray *bytes = NULL;

        if (!m_readers.contains(readerId))
            return NULL;

        m_bytePartsMutex.lock();
        int i = m_readers.value(readerId);

        if (m_byteParts.count() > i)
            bytes = m_byteParts.at(i);

        if (bytes)
            m_readers[readerId] = i + 1;
        m_bytePartsMutex.unlock();

        if (!bytes)
            return NULL;

        QBuffer *buffer = new QBuffer(bytes);
        buffer->open(QIODevice::ReadOnly);

        return buffer;
    }

    if (m_availableStreamsQueue.count())
        return m_availableStreamsQueue.takeFirst();

    return NULL;
}

void ProxyDownload::readReply(QIODevice *ioDevice)
{
    if (m_shareDownload) {
        m_bytePartsMutex.lock();
        m_byteParts.append(new QByteArray(ioDevice->readAll()));
        m_bytePartsMutex.unlock();
    } else {
        m_availableStreamsQueue.append(ioDevice);
    }

    emit bytePartAvailable();
}

void ProxyDownload::inputObjectFinished()
{
    emit downloadFinished();
}
