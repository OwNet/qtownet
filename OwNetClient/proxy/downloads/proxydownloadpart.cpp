#include "proxydownloadpart.h"
#include "messagehelper.h"

ProxyDownloadPart::ProxyDownloadPart(int nextDownloadPartIndex, QObject *parent) :
    QObject(parent),
    m_nextDownloadPartIndex(nextDownloadPartIndex),
    m_previousPart(NULL)
{
    connect(this, SIGNAL(destroySelf()), this, SLOT(deleteLater()));
}

void ProxyDownloadPart::deletePreviousPartAfterReadersRegister(ProxyDownloadPart *previousPart, QList<int> readerIds)
{
    m_previousPart = previousPart;
    m_readersOfPreviousParts.append(readerIds);
}

void ProxyDownloadPart::registerReader(int readerId)
{
    if (m_readersOfPreviousParts.contains(readerId)) {
        m_readersOfPreviousParts.removeAll(readerId);
        if (!m_readersOfPreviousParts.count() && m_previousPart) {
            delete m_previousPart;
            m_previousPart = NULL;
        }
    }
}
