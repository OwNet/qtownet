#include "proxywebinputobject.h"
#include "messagehelper.h"
#include "proxyrequest.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkProxy>
#include <QTcpSocket>
#include <QBuffer>

ProxyWebInputObject::ProxyWebInputObject(ProxyRequest *request, QObject *parent)
    : ProxyInputObject(request, parent), m_readHeaders(false)
{
}

void ProxyWebInputObject::readRequest()
{
    QNetworkAccessManager *manager = new QNetworkAccessManager(m_request->socket());
    QNetworkReply *reply = NULL;
    QNetworkRequest request;

    request.setUrl(m_request->qUrl());
    for (int i = 0; i < m_request->requestHeaders().count(); ++i)
        request.setRawHeader(m_request->requestHeaders().at(i).first.toLatin1(),
                             m_request->requestHeaders().at(i).second.toLatin1());
    request.setRawHeader("X-Proxied-By", "OwNet");

    switch (m_request->requestType()) {
    case ProxyRequest::GET:
        reply = manager->get(request);
        break;
    case ProxyRequest::POST:
        reply = manager->post(request, new QBuffer(new QByteArray(m_request->requestBody())));
        break;
    case ProxyRequest::PUT:
        reply = manager->put(request, new QBuffer(new QByteArray(m_request->requestBody())));
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

void ProxyWebInputObject::readReply()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

    if (!m_readHeaders) {
        m_readHeaders = true;

        m_httpStatusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toString();
        m_httpStatusDescription = reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();
        if (m_httpStatusDescription.isNull())
            m_httpStatusDescription = "";

        QList<QNetworkReply::RawHeaderPair> headers = reply->rawHeaderPairs();
        for (int i = 0; i < headers.count(); ++i)
            addHeader(headers.at(i).first, headers.at(i).second);
    }
    emit readyRead(reply);
}

void ProxyWebInputObject::error(QNetworkReply::NetworkError)
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    MessageHelper::debug(reply->errorString());

    disconnect(this);
    emit failed();
}

void ProxyWebInputObject::downloadFinished()
{
    disconnect(this);
    emit finished();
}
