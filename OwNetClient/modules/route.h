#ifndef ROUTE_H
#define ROUTE_H

#include<QString>
#include<QMap>
#include<QRegularExpressionMatch>

#include "irequest.h"

class IBus;

class Route
{
public:
    Route(QString url);

    typedef QRegularExpressionMatch Match;
    typedef std::function<QVariant* (IBus *bus, IRequest *req, Match params)> Callback;
    typedef QMap<IRequest::RequestType, Callback> CallbackMap;

    Route* on(IRequest::RequestType method, Callback callback);

    QString url(){ return m_url; }
    QRegularExpression* regexp(){ return &m_regexp; }
    CallbackMap* callbacks(){ return &m_callbacks; }

private:
    QString m_url;
    QRegularExpression m_regexp;
    CallbackMap m_callbacks;
};

#endif // ROUTE_H
