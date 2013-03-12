#include "refreshsession.h"

#include "isession.h"
#include "iproxyconnection.h"

#include <QTimer>

RefreshSession::RefreshSession(IProxyConnection *proxyConnection, QObject *parent) :
    QObject(parent),
    m_proxyConnection(proxyConnection)
{
}

void RefreshSession::start()
{
    QObject parent;
    ISession *session = m_proxyConnection->session(&parent);
    session->setValue("refresh_session", true);

    QTimer::singleShot(15 * 1000, this, SLOT(timeout()));
}

void RefreshSession::stop()
{
    QObject parent;
    ISession *session = m_proxyConnection->session(&parent);
    session->setValue("refresh_session", false);
}

void RefreshSession::timeout()
{
    stop();
}
