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

    virtual QVariant postBodyFromJson(bool *ok = NULL) const = 0;

    virtual IRequest::RequestType requestType() const = 0;
    virtual QString relativeUrl() const = 0;

    // virtual QString action() const = 0;
    virtual QString service() const = 0;
    virtual QString subDomain() const = 0;
    // virtual int id() const = 0;
    virtual bool hasParameter(const QString& key) const = 0;

    virtual QString parameterValue(const QString &key) const = 0;
    virtual void setParamater(const QString &, const QString &) {}
    virtual void setPostBody(const QVariantMap &) {}
};

#endif // IREQUEST_H
