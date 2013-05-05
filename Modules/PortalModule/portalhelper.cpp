#include "portalhelper.h"

#include <QCryptographicHash>
#include <QDateTime>
#include <QString>
#include <QByteArray>
#include <QSqlQuery>

#include "isession.h"
#include "iproxyconnection.h"

PortalHelper::PortalHelper(QObject *parent) :
    QObject(parent)
{
}

void PortalHelper::addSalt(QString *password, QString *salt)
{
    QByteArray current_time = QDateTime::currentDateTime().toString().toLatin1();
    QString salt_temp(QCryptographicHash::hash(current_time,QCryptographicHash::Sha1).toHex());

    *salt = salt_temp;
    QByteArray pass_plus_salt = (*password+salt_temp).toLatin1();
    QString salted_pass(QCryptographicHash::hash(pass_plus_salt,QCryptographicHash::Sha1).toHex());

    *password = salted_pass;

}

bool PortalHelper::isLoggedIn(IProxyConnection *proxyConnection)
{
    QObject parent;
    return !(proxyConnection->session(&parent)->value("logged").toString().isEmpty());
}

uint PortalHelper::currentUserId(IProxyConnection *proxyConnection)
{
    QObject parent;
    return proxyConnection->session(&parent)->value("logged").toUInt();
}
