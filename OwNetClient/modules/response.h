#ifndef RESPONSE_H
#define RESPONSE_H

#include "iresponse.h"
#include <QVariantMap>

class Response : public IResponse
{
public:
    Response();

    IResponse* setBody(const QVariant &body);
    IResponse* setStatus(Status status);
    IResponse* setStatus(const int code, const QString message);
    IResponse* setHeader(const QString key, const QString value);
    IResponse* setContentType(const QString type);

    QVariant body() { return m_body; }
    int status() { return m_httpStatusCode; }
    QString statusMessage() { return m_httpStatusMessage; }
    QMap<QString,QString> headers() { return m_headers; }

private:
    int m_httpStatusCode;
    QString m_httpStatusMessage;
    QMap<QString,QString> m_headers;
    QVariant m_body;

};

#endif // RESPONSE_H
