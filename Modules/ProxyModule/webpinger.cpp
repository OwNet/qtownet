#include "webpinger.h"

#include "isession.h"
#include "iproxyconnection.h"

#include <QSettings>
#include <QNetworkAccessManager>
#include <QNetworkReply>

WebPinger::WebPinger(IProxyConnection *proxyConnection, QObject *parent) :
    QObject(parent),
    m_proxyConnection(proxyConnection),
    m_nextUrlIndex(0),
    m_lastOK(false)
{
    QObject parentObject;
    QSettings *settings = m_proxyConnection->settings(&parentObject);
    settings->beginGroup("current_workspace");

    m_urlsToPing.append("http://www.google.com");
    m_urlsToPing.append(QString("http://yatta.fiit.stuba.sk/OwNetRestService/Ping/%1").arg(settings->value("id").toString()));
}

void WebPinger::ping()
{
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));
    connect(manager, SIGNAL(finished(QNetworkReply*)),
            manager, SLOT(deleteLater()));

    manager->get(QNetworkRequest(QUrl(nextUrl())));
}

void WebPinger::replyFinished(QNetworkReply *reply)
{
    QObject parent;
    bool ok = reply->error() == QNetworkReply::NoError;
    if (!ok && m_lastOK) {
        m_lastOK = ok;
        return;
    }
    m_proxyConnection->session(&parent)
            ->setValue("is_online", ok);
    m_lastOK = ok;
}

QString WebPinger::nextUrl()
{
    QString url = m_urlsToPing.at(m_nextUrlIndex);
    m_nextUrlIndex++;
    m_nextUrlIndex %= m_urlsToPing.count();
    return url;
}
