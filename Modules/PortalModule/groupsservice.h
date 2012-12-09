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

    void init(IRouter* router);

    QString name() const { return "groups"; }

    /**
     * @brief create creates new group
     * @param req
     * @return
     */
    IResponse *create( IRequest *req);

    /**
     * @brief show returns detail of the group with specified id
     * @param req
     * @param id
     * @return
     */
    IResponse *show(IRequest *req, int id);

    /**
     * @brief index returns list of groups
     * @param req
     * @return
     */
    IResponse *index( IRequest *req);

    /**
     * @brief edit edits group with specified id, only admin of group can do this action
     * @param req
     * @param id
     * @return
     */
    IResponse *edit( IRequest *req, int id);

    IResponse *del( IRequest *req, int id);

private:

    bool isMember(int user_id, int group_id);

    bool isAdmin(int user_id, int group_id);

    IResponse *joinGroup(IRequest *req);

    IResponse *approveUser(IRequest *req);

    IResponse *addAdmin(IRequest *req);

    /**
     * @brief getApprovements returns awaiting approvements for group,
     * can be requested only by admin of specific group
     * @param req
     * @return
     */
    IResponse *getApprovements(IRequest *req);

    /**
     * @brief getGroupUsers returns list of group's users
     * @param req
     * @return
     */
    IResponse *getGroupUsers( IRequest *req);

    /**
     * @brief getUsersGroups returns user's groups
     * @param req
     * @return
     */
    IResponse *getUsersGroups( IRequest *req);

    IResponse *getGroupTypes(IRequest *req);

    IResponse *deleteUser(IRequest *req);

    IProxyConnection *m_proxyConnection;
};

#endif // GROUPSSERVICE_H
