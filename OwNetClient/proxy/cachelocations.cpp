#include "cachelocations.h"

#include "proxywebdownload.h"
#include "session.h"
#include "databasesettings.h"

#include <QDateTime>
#include <QStringList>

CacheLocations::CacheLocations(QObject *parent) :
    QObject(parent),
    m_local(NULL),
    m_currentDownload(NULL)
{
}

void CacheLocations::addLocation(const QString &clientId, const QString &dateTime, ProxyWebDownload *location)
{
    addLocation(clientId, QDateTime::fromString(dateTime, Qt::ISODate), location);
}

void CacheLocations::addLocation(const QString &clientId, const QDateTime &dateTime, ProxyWebDownload *location)
{
    QDateTime roundedDateTime;
    roundedDateTime.setDate(dateTime.date());
    QTime time = dateTime.time();
    roundedDateTime.setTime(QTime(time.hour(), 0));

    QString key = roundedDateTime.toString(Qt::ISODate);

    if (m_locations.contains(key)) {
        QList<ProxyWebDownload*> *list = m_locations.value(key);
        if (location->isLocalCache()) {
            list->prepend(location);
        } else if (location->isWeb()) {
            list->append(location);
        } else {
            bool inserted = false;
            for (int i = 0; i < list->count(); ++i) {
                if (list->at(i)->isLocalCache())
                    continue;
                list->insert(i, location);
                inserted = true;
                break;
            }
            if (!inserted)
                list->append(location);
        }
        m_locations.value(key)->append(location);
    } else {
        QList<ProxyWebDownload*> *list = new QList<ProxyWebDownload*>();
        list->append(location);
        m_locations.insert(key, list);
    }

    if (location->isLocalCache())
        m_local = location;
}

void CacheLocations::removeLocation(const QString &clientId)
{
    foreach (QString key, m_locations.keys()) {
        QList<ProxyWebDownload *> *list = m_locations.value(key);
        for (int i = list->count() - 1; i >= 0; --i) {
            if (list->at(i)->clientId() == clientId)
                list->removeAt(i);
        }
    }
}

bool CacheLocations::containsLocation(const QString &clientId) const
{
    return m_locations.contains(clientId);
}

QIODevice *CacheLocations::getStream(ProxyRequest *request, ProxyWebReader *reader, ProxyHandlerSession *handlerSession, bool *finished, bool refresh)
{
    if (m_currentDownload)
        return m_currentDownload->getStream(reader, handlerSession, finished);

    ProxyWebDownload *download = NULL;
    QString downloadClientId;
    QVariantMap availableClients = Session().availableClients();
    if (!refresh) {
        bool isOnline = Session().isOnline();
        QList<ProxyWebDownload*> locations = sortedLocations();
        foreach (ProxyWebDownload *location, locations) {
            if (location->isLocalCache()) {
                if (location->exists()) {
                    download = location;
                    break;
                }
            } else if (location->isNetworkCache()) {
                if (!availableClients.contains(location->clientId()))
                    continue;

                downloadClientId = download->clientId();
                break;
            } else {
                if (location->isWeb() && !isOnline)
                    continue;
                break;
            }
        }
    }

    if (!download) {
        if (m_local) {
            m_local->downloadFromTheWebOrNetwork(request, downloadClientId);
            download = m_local;
        } else {
            download = new ProxyWebDownload(request);
            download->downloadFromTheWebOrNetwork(request, downloadClientId);
        }
        addLocation(DatabaseSettings().clientId(), QDateTime::currentDateTime(), download);
        m_currentDownload = download;
        connect(download, SIGNAL(finished()), this, SLOT(currentDownloadFinished()));
        connect(download, SIGNAL(failed()), this, SLOT(currentDownloadFailed()));
    }

    return download->getStream(reader, handlerSession, finished);
}

QList<ProxyWebDownload *> CacheLocations::sortedLocations() const
{
    QList<ProxyWebDownload *> results;
    QStringList keys(m_locations.keys());
    keys.sort();

    for (int i = keys.count() - 1; i >= 0; --i) {
        results.append(*(m_locations.value(keys.at(i))));
    }

    return results;
}

void CacheLocations::currentDownloadFinished()
{
    //m_currentDownload->disconnect(this);
    m_currentDownload = NULL;
}

void CacheLocations::currentDownloadFailed()
{
    //m_currentDownload->disconnect(this);
    m_currentDownload = NULL;
}
