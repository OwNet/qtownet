#include "proxywebinputobject.h"
#include "messagehelper.h"
#include "proxyrequest.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>

ProxyWebInputObject::ProxyWebInputObject(ProxyRequest *request, QObject *parent)
    : ProxyInputObject(request, parent), m_readHeaders(false)
{
}

void ProxyWebInputObject::readRequest()
{
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkReply *reply = NULL;
    QNetworkRequest request;
    request.setUrl(QUrl(m_request->url()));
    for (int i = 0; i < m_request->requestHeaders().count(); ++i)
        request.setRawHeader(m_request->requestHeaders().at(i).first.toLatin1(),
                             m_request->requestHeaders().at(i).second.toLatin1());
    request.setRawHeader("X-Proxied-By", "OwNet");

    if (m_request->requestType() == ProxyRequest::GET)
        reply = manager->get(request);
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

        QList<QNetworkReply::RawHeaderPair> headers = reply->rawHeaderPairs();
        for (int i = 0; i < headers.count(); ++i)
            addHeader(headers.at(i).first, headers.at(i).second);
    }
    emit readyRead(reply, this);
}

void ProxyWebInputObject::error(QNetworkReply::NetworkError)
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    MessageHelper::debug(reply->errorString());

    disconnect(this);
    emit finished();
}

void ProxyWebInputObject::downloadFinished()
{
    disconnect(this);
    emit finished();
}
