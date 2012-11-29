#include "loggedpage.h"

LoggedPage::LoggedPage(int id, QString url)
    :  m_id(id), m_url(url)
{

}

QString LoggedPage::link()
{
    if (m_links.empty())
    {
        return QString("");
    }
    QString ret(m_links.last());
    m_links.removeLast();
    return ret;
}

const int LoggedPage::id()
{
    return m_id;
}
