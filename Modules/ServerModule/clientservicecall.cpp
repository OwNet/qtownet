#include "clientservicecall.h"

#include "iproxyconnection.h"

#include <QUrl>
#include <QUrlQuery>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkProxy>
#include <QDebug>
#include <QEventLoop>
#include <QBuffer>
#include <QStringList>

ClientServiceCall::ClientServiceCall(IProxyConnection *proxyConnection, QObject *parent) :
    QObject(parent),
    m_proxyConnection(proxyConnection)
{
}

QVariant ClientServiceCall::callClientService(int clientId, const QString &apiUrl, IRequest *request)
{
    QUrlQuery urlQuery(QString("http://external.ownet/api/%1").arg(apiUrl));
    /*foreach (QString key, parameters.keys()) {
        urlQuery.addQueryItem(key, parameters.value(key).toString());
    }*/

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    manager->setProxy(QNetworkProxy(QNetworkProxy::HttpProxy, "localhost", 8081));
    QNetworkReply *reply = NULL;
    QNetworkRequest networkRequest(QUrl(urlQuery.toString(QUrl::FullyEncoded)));

    switch (request->requestType()) {
    case IRequest::GET:
        reply = manager->get(networkRequest);
        break;
    case IRequest::POST:
        networkRequest.setRawHeader("Content-type", "application/json");
        reply = manager->post(networkRequest, m_proxyConnection->toJson(request->postBodyFromJson()));
        break;
    case IRequest::PUT:
        networkRequest.setRawHeader("Content-type", "application/json");
        reply = manager->put(networkRequest, m_proxyConnection->toJson(request->postBodyFromJson()));
        break;
    case IRequest::DELETE:
        reply = manager->deleteResource(networkRequest);
        break;
    case IRequest::UNKNOWN:
        return;
    }

    // Wait for the request to finish
    QEventLoop loop;
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    bool ok = false;
    QVariant json = m_proxyConnection->fromJson(reply->readAll(), &ok);
    if (ok)
        return json;
    return QVariant();
}
