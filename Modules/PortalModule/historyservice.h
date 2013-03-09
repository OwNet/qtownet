#ifndef HISTORYSERVICE_H
#define HISTORYSERVICE_H

#include <QObject>

#include "irestservice.h"
#include "iservice.h"
#include "iresponse.h"

class IProxyConnection;
class IDatabaseUpdateQuery;
class HistoryModule;

class HistoryService : public QObject, public IService
{
    Q_OBJECT
public:
    HistoryService(IProxyConnection *proxyConnection, QObject *parent = 0);

    void init(IRouter *router);

    QString name() const { return "history"; }

    IResponse *visit(IRequest *req);

private:

   IProxyConnection *m_proxyConnection;



   bool registerPageQuery(QString url, QString title, uint &id);
   bool registerVisitQuery(int user_id, uint page_id);
   bool registerEdgeQuery(uint page_from_id, uint page_to_id);
   bool registerTraverseQuery(int user_id, uint page_from_id, uint page_to_id);
};


#endif // HISTORYSERVICES_H
