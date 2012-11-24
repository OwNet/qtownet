#include "stubrequest.h"

#include <QUrl>
#include <QStringList>

StubRequest::StubRequest(QObject *parent) :
    QObject(parent)
{
}

QVariantMap StubRequest::postBodyFromJson() const
{
    return QVariantMap();
}

QMap<QString, QString> StubRequest::postBodyFromForm() const
{
    return QMap<QString, QString>();
}

IRequest::RequestType StubRequest::requestType() const
{
    return GET;
}

QVariantMap StubRequest::requestHeaders() const
{
    return QVariantMap();

}

QUrl StubRequest::qUrl() const
{
    return QUrl();
}

QString StubRequest::url() const
{
    return QString();
}

QString StubRequest::requestContentType(const QString &defaultContentType, const QString &extension) const
{
    return QString();
}

QString StubRequest::relativeUrl() const
{
    return QString();
}

QString StubRequest::action() const
{
    return QString();
}

void StubRequest::setModule(QString m)
{
    m_module = m;
}

QString StubRequest::module() const
{
    return m_module;
}

QString StubRequest::subDomain() const
{
    return QString();
}


int StubRequest::id() const
{
    return 1;
}

QString StubRequest::parameterValue(const QString &key) const
{
    return QString();
}

QStringList StubRequest::allParameterValues(const QString &key) const
{
    return QStringList();
}

QByteArray StubRequest::requestBody() const
{
    return QByteArray();
}

QString StubRequest::staticResourcePath() const
{
    return QString();
}


int StubRequest::hashCode() const
{
    return 0;
}

bool StubRequest::isLocalRequest() const
{
    return false;
}

bool StubRequest::isStaticResourceRequest() const
{
    return false;
}

bool StubRequest::isApiRequest() const
{
    return true;
}
