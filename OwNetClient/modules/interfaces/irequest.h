#ifndef IREQUEST_H
#define IREQUEST_H

#include <QVariantMap>

class IRequest
{
public:
    enum RequestType {
        GET,
        POST,
        PUT,
        DELETE,
        UNKNOWN
    };

    virtual QVariantMap postBodyFromJson() const = 0;
    virtual QMap<QString, QString> postBodyFromForm() const = 0;

    virtual IRequest::RequestType requestType() const = 0;
    virtual QVariantMap requestHeaders() const = 0;

    virtual QUrl qUrl() const = 0;
    virtual QString url() const = 0;
    virtual QString requestContentType(const QString &defaultContentType = "", const QString &extension = "") const = 0;
    virtual QString relativeUrl() const = 0;
    virtual QString action() const = 0;
    virtual QString module() const = 0;
    virtual QString subDomain() const = 0;

    virtual int id() const = 0;
    virtual QString parameterValue(const QString &key) const = 0;
    virtual bool hasParameter(const QString& key) const = 0;
    virtual QStringList allParameterValues(const QString &key) const = 0;
    virtual QByteArray requestBody() const = 0;
    virtual QString staticResourcePath() const = 0;

    virtual int hashCode() const = 0;

    virtual bool isLocalRequest() const = 0;
    virtual bool isStaticResourceRequest() const = 0;
    virtual bool isApiRequest() const = 0;
};

#endif // IREQUEST_H
