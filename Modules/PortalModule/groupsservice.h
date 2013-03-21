#ifndef GROUPSSERVICE_H
#define GROUPSSERVICE_H

#include <QObject>

#include "irestservice.h"


class IProxyConnection;

/**
 * @brief The GroupsService class
 */
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
    IResponse *show(IRequest *req, const QString &textId);

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
    IResponse *edit(IRequest *req, const QString &textId);

    IResponse *del(IRequest *req, const QString &textId);

    /**
     * @brief allCount
     * @param req
     * @return count of pages for all groups
     */
    IResponse *allPagesCount(IRequest *req);
    IResponse *myPagesCount(IRequest *req);

    IResponse *myAdminPagesCount(IRequest *req);
    IResponse *getMyAdminGroups(IRequest *req);

    IResponse *getNotMyGroups(IRequest *req);
    IResponse *notMyPagesCount(IRequest *req);

    IResponse *declineUser(IRequest *req);

    IResponse *getAwaitingGroups(IRequest *req);
    IResponse *awaitingPagesCount(IRequest *req);
private:

    bool isMember(uint user_id, uint group_id);

    bool isAdmin(uint user_id, uint group_id);

    bool checkGroupPassword(QString password, QString group_id);

    IResponse *getIsAdmin( IRequest *req );

     IResponse *getIsMember( IRequest *req);

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
