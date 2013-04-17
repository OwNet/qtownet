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
#include <QThread>
#include <QStringList>

ClientServiceCall::ClientServiceCall(IProxyConnection *proxyConnection, QObject *parent) :
    QObject(parent),
    m_proxyConnection(proxyConnection),
    m_networkAccessManager(NULL),
    m_request(NULL),
    m_reply(NULL),
    m_response(NULL),
    m_proxyPort(0)
{
}

IResponse *ClientServiceCall::callClientService(const QString &clientId, const QString &apiUrl, IRequest *request)
{
    QObject parent;
    ISession *session = m_proxyConnection->session(&parent);
    QStringList ipPort = session->availableClients().value(clientId).toString().split(":");

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
    ClientServiceCall *call = new ClientServiceCall(m_proxyConnection);
    call->setProxy(ip, port);

    return callService(QString("http://external.ownet/api/%1").arg(apiUrl),
                       request, call);
}

IResponse *ClientServiceCall::callCentralService(const QString &relativeUrl, IRequest *request)
{
    return callService(QString("http://yatta.fiit.stuba.sk/OwNetRestService/%1").arg(relativeUrl),
                       request);
}

IResponse *ClientServiceCall::callService(const QString &url, IRequest *request, ClientServiceCall *clientServiceCall)
{
    if (!clientServiceCall)
        clientServiceCall = new ClientServiceCall(m_proxyConnection);

    QUrlQuery urlQuery(url);
    QMap<QString, QString> parameters = request->paramaters();
    foreach (QString key, parameters.keys())
        urlQuery.addQueryItem(key, parameters.value(key));

    clientServiceCall->setUrl(urlQuery.toString(QUrl::FullyEncoded));
    clientServiceCall->setRequest(request);

    QThread* thread = new QThread;
    clientServiceCall->moveToThread(thread);
    connect(thread, SIGNAL(started()), clientServiceCall, SLOT(startRequest()), Qt::QueuedConnection);
    connect(clientServiceCall, SIGNAL(requestFinished()), thread, SLOT(quit()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();

    clientServiceCall->waitForRequest();

    IResponse *response = clientServiceCall->response();
    delete clientServiceCall;
    return response;
}

void ClientServiceCall::setProxy(const QString &address, int port)
{
    m_proxyAddress = address;
    m_proxyPort = port;
}

void ClientServiceCall::startRequest()
{
    m_networkAccessManager = new QNetworkAccessManager;
    if (!m_proxyAddress.isEmpty())
        m_networkAccessManager->setProxy(QNetworkProxy(QNetworkProxy::HttpProxy, m_proxyAddress, m_proxyPort));
    QNetworkRequest networkRequest((QUrl(m_url)));

    switch (m_request->requestType()) {
    case IRequest::GET:
        m_reply = m_networkAccessManager->get(networkRequest);
        break;
    case IRequest::POST:
        networkRequest.setRawHeader("Content-type", "application/json");
        m_reply = m_networkAccessManager->post(networkRequest, m_proxyConnection->toJson(m_request->postBodyFromJson()));
        break;
    case IRequest::PUT:
        networkRequest.setRawHeader("Content-type", "application/json");
        m_reply = m_networkAccessManager->put(networkRequest, m_proxyConnection->toJson(m_request->postBodyFromJson()));
        break;
    case IRequest::DELETE:
        m_reply = m_networkAccessManager->deleteResource(networkRequest);
        break;
    default:
        m_reply = m_networkAccessManager->get(networkRequest);
        break;
    }

    connect(this, SIGNAL(requestFinished()), m_networkAccessManager, SLOT(deleteLater()));
    connect(m_reply, SIGNAL(finished()), this, SLOT(readyRead()));
}

void ClientServiceCall::readyRead()
{
    m_reply->waitForReadyRead(15000);

    bool ok = false;
    if (m_reply->error() == QNetworkReply::NoError) {
        QVariant json = m_proxyConnection->fromJson(m_reply->readAll(), &ok);
        if (ok)
            m_response = m_request->response(json);
        else
            m_response = m_request->response(IResponse::OK);
    } else {
        m_response = m_request->response(IResponse::NOT_FOUND);
    }
    emit requestFinished();
    m_semaphore.release();
}
