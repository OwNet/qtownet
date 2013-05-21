#ifndef DOWNLOADORDERSSERVICE_H
#define DOWNLOADORDERSSERVICE_H

#include <QObject>

#include "irestservice.h"

class IProxyConnection;

/**
 * @brief The GroupsService class
 */

class DownloadOrdersService :  public QObject, public IRestService
{
public:
    explicit DownloadOrdersService(IProxyConnection *proxyConnection, QObject *parent = 0);

    void init(IRouter* router);

    QString name() const { return "orders"; }

    /**
     * @brief Create creates new download order. Not yet supported.
     * @param req
     * @return
     */
    IResponse *create( IRequest *req);

    /**
     * @brief Show returns details of the download order specified by id. Not yet supported.
     * @param req
     * @param id
     * @return
     */
    IResponse *show(IRequest *req, uint id);

    /**
     * @brief Index returns list of download orders.
     * @param req
     * @return
     */
    IResponse *index( IRequest *req);

    /**
     * @brief Edit is not yet supported.
     * @param req
     * @param id
     * @return
     */
    IResponse *edit(IRequest *req, uint id);

    /**
     * @brief Delete deletes the download order specified by id.
     * @param req
     * @param id
     * @return
     */
    IResponse *del(IRequest *req, uint id);

    /**
     * @brief allPagesCount
     * @param req
     * @return count of pages for all groups
     */
    IResponse *allPagesCount(IRequest *req);

private:

    static const int PER_PAGE = 10;
    static const int ORDER_PRIORITY = 10000;
    IProxyConnection *m_proxyConnection;
    bool registerOrderQuery(QString title, QString url);

};

#endif // DOWNLOADORDERSSERVICE_H
