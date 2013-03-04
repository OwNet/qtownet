#include "clientservicecall.h"

#include "iproxyconnection.h"
#include "isession.h"

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

IResponse *ClientServiceCall::callClientService(uint clientId, const QString &apiUrl, IRequest *request)
{
    QObject parent;
    ISession *session = m_proxyConnection->session(&parent);
    QStringList ipPort = session->availableClients().value(QString::number(clientId)).toString().split(":");

    if (ipPort.count() < 2)
        return request->response(IResponse::SERVICE_UNAVAILABLE);

    QString ip = ipPort.takeFirst();
    int port = ipPort.takeFirst().toInt();

    if (ip.isEmpty())
        return request->response(IResponse::SERVICE_UNAVAILABLE);

    return callClientService(ip, port, apiUrl, request);
}

IResponse *ClientServiceCall::callClientService(const QString &ip, int port, const QString &apiUrl, IRequest *request)
{
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    manager->setProxy(QNetworkProxy(QNetworkProxy::HttpProxy, ip, port));

    return callService(QString("http://external.ownet/api/%1").arg(apiUrl),
                       request,
                       manager);
}

IResponse *ClientServiceCall::callCentralService(const QString &relativeUrl, IRequest *request)
{
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);

    return callService(QString("http://yatta.fiit.stuba.sk/OwNetRestService/%1").arg(relativeUrl),
                       request,
                       manager);
}

IResponse *ClientServiceCall::callService(const QString &url, IRequest *request, QNetworkAccessManager *manager)
{
    QUrlQuery urlQuery(url);
    /*foreach (QString key, parameters.keys()) {
        urlQuery.addQueryItem(key, parameters.value(key).toString());
    }*/

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
        return request->response(IResponse::BAD_REQUEST);
    }

    // Wait for the request to finish
    QEventLoop loop;
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    bool ok = false;

    if (reply->error() == QNetworkReply::NoError) {
        QVariant json = m_proxyConnection->fromJson(reply->readAll(), &ok);
        if (ok)
            return request->response(json);
        return request->response(IResponse::OK);
    }
    return request->response(IResponse::NOT_FOUND);
}
