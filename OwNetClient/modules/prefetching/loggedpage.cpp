#include "loggedpage.h"

LoggedPage::LoggedPage(int id, QString url)
    :  m_id(id), m_url(url)
{

}

const int LoggedPage::id()
{
    return m_id;
}
