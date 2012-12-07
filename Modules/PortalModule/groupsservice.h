#ifndef GROUPSSERVICE_H
#define GROUPSSERVICE_H

#include <QObject>

#include "irestservice.h"

class IProxyConnection;

class GroupsService : public QObject, public IRestService
{
    Q_OBJECT
public:
    explicit GroupsService(IProxyConnection *proxyConnection, QObject *parent = 0);

    QString name() const { return "groups"; }

    // create element
    QVariant *create(IBus *bus, IRequest *req);

    // show element
    QVariant *show(IBus *bus, IRequest *req, int id);

    QVariant *index(IBus *bus, IRequest *req);

    QVariant *processRequest(IBus *bus, IRequest *req);

    QVariant *edit(IBus *bus, IRequest *req, int id);

    QVariant *del(IBus *bus, IRequest *req, int id);

private:

    bool isMember(int user_id, int group_id);

    bool isAdmin(int user_id, int group_id);

    QVariant *joinGroup(IBus *bus, IRequest *req);

    QVariant *approveUser(IBus *bus, IRequest *req);

    QVariant *addAdmin(IBus *bus, IRequest *req);

    //approvements for specific group, needs group_id in json
    QVariant *getApprovements(IBus *bus, IRequest *req);

    QVariant *getGroupUsers(IBus *bus, IRequest *req);

    QVariant *getUsersGroups(IBus *bus, IRequest *req);

    QVariant *getGroupTypes(IBus *bus, IRequest *req);

    QVariant *deleteUser(IBus *bus, IRequest *req);

    IProxyConnection *m_proxyConnection;
};

#endif // GROUPSSERVICE_H
