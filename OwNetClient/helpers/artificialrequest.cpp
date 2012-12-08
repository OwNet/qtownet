#include "artificialrequest.h"
#include "response.h"

ArtificialRequest::ArtificialRequest(IRequest::RequestType requestType, const QString &service, const QString &url, QObject *parent) :
    QObject(parent),
    m_requestType(requestType),
    m_service(service)
{
    m_url = "/"+service+"/"+url;
}

ArtificialRequest::ArtificialRequest(IRequest::RequestType requestType, const QString &service, const int id, QObject* parent) :
    QObject(parent),
    m_requestType(requestType),
    m_service(service)
{
    m_url = "/"+service+"/"+QString::number(id);
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
