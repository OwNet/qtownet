#ifndef ROUTE_H
#define ROUTE_H

#include<QString>
#include<QMap>
#include<QRegularExpressionMatch>

#include "irequest.h"
#include "iroute.h"


class Route : public IRoute
{
public:
    Route(QString url);

    Route* on(IRequest::RequestType method, IRoute::Callback callback);

    QString url(){ return m_url; }
    QRegularExpression* regexp(){ return &m_regexp; }
    IRoute::CallbackMap* callbacks(){ return &m_callbacks; }

private:
    QString m_url;
    QRegularExpression m_regexp;
    IRoute::CallbackMap m_callbacks;
};

#endif // ROUTE_H
