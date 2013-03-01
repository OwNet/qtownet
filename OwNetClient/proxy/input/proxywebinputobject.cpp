#include "proxywebinputobject.h"
#include "messagehelper.h"
#include "proxyrequest.h"
#include "proxydownloads.h"
#include "proxytrafficcounter.h"
#include "databaseselectquery.h"
#include "databasesettings.h"
#include "session.h"
#include "idatabaseselectquerywheregroup.h"

#include <QNetworkReply>
#include <QNetworkProxy>
#include <QTcpSocket>
#include <QBuffer>

ProxyWebInputObject::ProxyWebInputObject(ProxyRequest *request, QObject *parent)
    : ProxyInputObject(request, parent), m_readHeaders(false), m_retryIfFailed(false)
{
}

void ProxyWebInputObject::readRequest()
{
    uint myId = DatabaseSettings().clientId();

    DatabaseSelectQuery query("client_caches");
    IDatabaseSelectQueryWhereGroup *where = query.whereGroup(IDatabaseSelectQuery::And);
    where->where("cache_id", m_request->hashCode());
    where->where("client_id", myId, IDatabaseSelectQuery::NotEqual);
    query.orderBy("");
    while (query.next())
        if (isClientOnline(query.value("client_id").toUInt()))
            m_clientsToTry.append(query.value("client_id").toUInt());

    createReply();

    ProxyDownloads::instance()->trafficCounter()->increaseCurrentTraffic();
}

void ProxyWebInputObject::readReply()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

    if (!m_readHeaders)
        readResponseHeaders(reply);

    emit readyRead(reply);
}

void ProxyWebInputObject::error(QNetworkReply::NetworkError)
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

    if (m_retryIfFailed) {
        reply->deleteLater();
        m_retryIfFailed = false;
        createReply();
        return;
    }

    if (!m_readHeaders) {
        readResponseHeaders(reply);

        emit readyRead(new QBuffer(this));
    }
    MessageHelper::debug(reply->errorString());

    disconnect(this);
    emit failed();
}

void ProxyWebInputObject::downloadFinished()
{
    if (!m_readHeaders) {
        QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
        readResponseHeaders(reply);

        emit readyRead(new QBuffer(this));
    }

    disconnect(this);
    emit finished();
}

void ProxyWebInputObject::readResponseHeaders(QNetworkReply *reply)
{
    m_readHeaders = true;

    m_httpStatusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toString();
    m_httpStatusDescription = reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();
    if (m_httpStatusDescription.isNull())
        m_httpStatusDescription = "";

    QList<QNetworkReply::RawHeaderPair> headers = reply->rawHeaderPairs();
    for (int i = 0; i < headers.count(); ++i)
        addHeader(headers.at(i).first, headers.at(i).second);
}

void ProxyWebInputObject::createReply()
{
    QNetworkAccessManager *manager = new QNetworkAccessManager(parent());
    QNetworkReply *reply = NULL;
    QNetworkRequest request;

    if (m_clientsToTry.count()) {
        manager->setProxy(QNetworkProxy(QNetworkProxy::HttpProxy, clientIp(m_clientsToTry.takeFirst()), 8081));
        m_retryIfFailed = true;
    }

    request.setUrl(QUrl(m_request->url()));
    foreach (QString headerName, m_request->requestHeaders().keys())
        request.setRawHeader(headerName.toUtf8(),
                             m_request->requestHeaders().value(headerName).toString().toUtf8());
    request.setRawHeader("X-Proxied-By", "OwNet");

    switch (m_request->requestType()) {
    case ProxyRequest::GET:
        reply = manager->get(request);
        break;
    case ProxyRequest::POST:
        reply = manager->post(request, new QBuffer(new QByteArray(m_request->requestBody()), this));
        break;
    case ProxyRequest::PUT:
        reply = manager->put(request, new QBuffer(new QByteArray(m_request->requestBody()), this));
        break;
    case ProxyRequest::DELETE:
        reply = manager->deleteResource(request);
        break;
    case ProxyRequest::UNKNOWN:
        break;
    }

    if (reply == NULL) {
        emit finished();
        return;
    }

    connect(reply, SIGNAL(finished()), this, SLOT(downloadFinished()));
    connect(reply, SIGNAL(readyRead()), this, SLOT(readReply()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error(QNetworkReply::NetworkError)));
}

bool ProxyWebInputObject::isClientOnline(uint clientId) const
{
    return Session().availableClients().contains(QString::number(clientId));
}

QString ProxyWebInputObject::clientIp(uint clientId) const
{
    return Session().availableClients().value(QString::number(clientId)).toString();
}
