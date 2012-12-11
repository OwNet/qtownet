#include "response.h"

Response::Response() :
    m_httpStatusCode(-1),
    m_httpStatusMessage(""),
    m_body(QVariant())
{
}

IResponse *Response::setBody(const QVariant &body)
{
    m_body = body;
    return this;
}

IResponse *Response::setStatus(IResponse::Status status)
{
    switch(status) {

    case OK :
        return setStatus(OK,"OK");

    case CREATED :
        return setStatus(CREATED,"Created");

    case NO_CONTENT :
        return setStatus(NO_CONTENT,"No Content");

    case BAD_REQUEST :
        return setStatus(BAD_REQUEST,"Bad Request");

    case UNAUTHORIEZED :
        return setStatus(UNAUTHORIEZED,"Unathorized");

    case FORBIDDEN :
        return setStatus(FORBIDDEN,"Forbidden");

    case NOT_FOUND :
        return setStatus(NOT_FOUND,"Not Found");

    case METHOD_NOT_ALLOWED :
        return setStatus(METHOD_NOT_ALLOWED,"Method Not Allowed");

    case CONFLICT :
        return setStatus(CONFLICT,"Conflict");

    case INTERNAL_SERVER_ERROR :
        return setStatus(INTERNAL_SERVER_ERROR,"Internal Server Error");

    case NOT_IMPLEMENTED :
        return setStatus(NOT_IMPLEMENTED,"Not Implemented");

    case SERVICE_UNAVAILABLE :
        return setStatus(503,"Service Unavailable");
    }

    return this;
}

IResponse *Response::setStatus(const int code, const QString message)
{
    m_httpStatusCode = code;
    m_httpStatusMessage = message;
    return this;
}

IResponse *Response::setHeader(const QString key, const QString value)
{
    m_headers.insert(key,value);
    return this;
}

IResponse *Response::setContentType(const QString type)
{
    return setHeader("Content-Type", type);
}
