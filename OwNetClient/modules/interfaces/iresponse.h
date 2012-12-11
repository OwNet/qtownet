#ifndef IRESPONSE_H
#define IRESPONSE_H

#include <QString>
#include <QMap>

class QVariant;

// const QVariant &body, int httpStatusCode=200, const QString &httpStatusMessage=QString()

class IResponse
{
public:

    enum Status {
        OK = 200,
        CREATED = 201,
        NO_CONTENT = 204,
        BAD_REQUEST = 400,
        UNAUTHORIEZED = 401,
        FORBIDDEN = 403,
        NOT_FOUND = 404,
        METHOD_NOT_ALLOWED = 405,
        CONFLICT = 409,
        INTERNAL_SERVER_ERROR = 500,
        NOT_IMPLEMENTED = 501,
        SERVICE_UNAVAILABLE = 503
    };

    virtual IResponse* setBody(const QVariant &body) = 0;
    virtual IResponse* setStatus(Status status) = 0;
    virtual IResponse* setStatus(const int code, const QString message) = 0;
    virtual IResponse* setHeader(const QString key, const QString value) = 0;
    virtual IResponse* setContentType(const QString type) = 0;

    virtual QVariant body() = 0;
    virtual int status() = 0;
    virtual QString statusMessage() = 0;
    virtual QMap<QString,QString> headers() = 0;

};

#endif // IRESPONSE_H
