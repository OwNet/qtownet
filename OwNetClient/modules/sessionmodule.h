#ifndef SESSIONMODULE_H
#define SESSIONMODULE_H

#include <QObject>
#include <QVariantMap>
#include <QByteArray>
#include <imodule.h>

class SessionModule : public IModule
{
    Q_OBJECT
public:
    explicit SessionModule(QObject *parent = 0);

    // check whether user is logged in or not
    bool isLoggedIn();
    
signals:
    
public slots:

private:

    // create session
    QByteArray* create(IBus *bus, ProxyRequest *req);

    //delete element
    QByteArray* del( IBus *bus, ProxyRequest *req);

   /* // show element
    QByteArray* show( IBus *bus, ProxyRequest *req);

    //edit element
    QByteArray* edit( IBus *bus, ProxyRequest *req);

    QByteArray* index( IBus *bus,  ProxyRequest *req);*/

    QVariantMap m_sessionData;

    
};

#endif // SESSIONMODULE_H
