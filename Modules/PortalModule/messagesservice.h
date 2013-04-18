#ifndef MESSAGESSERVICE_H
#define MESSAGESSERVICE_H

#include <QObject>

#include "iuidrestservice.h"
#include "activitymanager.h"

class IProxyConnection;

class MessagesService : public QObject, public IUidRestService
{
    Q_OBJECT
public:
    explicit MessagesService(IProxyConnection *proxyConnection, QObject *parent = 0);

    void init(IRouter* router);

    QString name() const { return "messages"; }

    IResponse *create( IRequest *req);
    IResponse *show(IRequest *req, const QString &uid);
    IResponse  *index(IRequest *req);
    IResponse  *del(IRequest *req, const QString &uid);

    IResponse *allPagesCount(IRequest *req);
private:
    IProxyConnection *m_proxyConnection;
    ActivityManager *m_activityManager;
};

#endif // MESSAGESSERVICE_H
