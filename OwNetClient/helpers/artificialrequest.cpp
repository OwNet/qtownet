#include "artificialrequest.h"
#include "response.h"

#include "proxyconnection.h"

ArtificialRequest::ArtificialRequest(IRequest::RequestType requestType, const QString &service, const QString &url, QObject *parent) :
    QObject(parent),
    m_requestType(requestType),
    m_service(service)
{
    m_url = "/api/"+service+"/"+url;
}

ArtificialRequest::ArtificialRequest(IRequest::RequestType requestType, const QString &service, const int id, QObject* parent) :
    QObject(parent),
    m_requestType(requestType),
    m_service(service)
{
    m_url = "/api/"+service+"/"+QString::number(id);
}

QVariant ArtificialRequest::postBodyFromJson(bool *ok) const
{
    if (ok)
        *ok = true;

    return m_postBody;
}

QString ArtificialRequest::relativeUrl() const
{
    QString path = m_url;
    if (path.startsWith('/'))
        path.remove(0, 1);
    return path;
}

IResponse *ArtificialRequest::response()
{
    return new Response();
}

IResponse *ArtificialRequest::response(const QVariant body, IResponse::Status status)
{
    return (new Response())->setBody(body)->setStatus(status);
}

IResponse *ArtificialRequest::response(IResponse::Status status)
{
    return (new Response())->setStatus(status);
}

IProxyConnection *ArtificialRequest::proxyConnection()
{
    return new ProxyConnection(this);
}
