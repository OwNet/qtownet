#include "artificialrequest.h"

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
