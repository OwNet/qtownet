#include "route.h"
#include "irequest.h"

Route::Route(QString url) :
    m_url(url)
{
    QString pattern = "^" + QRegularExpression::escape(url).replace(QRegularExpression("\\\\:([^:/\\\\]+)"),"(?<\\1>[^/]+)") + "$";
    m_regexp = QRegularExpression(pattern);
}

Route* Route::on(IRequest::RequestType method, Callback callback)
{
    m_callbacks.insert(method,callback);
    return this;
}
