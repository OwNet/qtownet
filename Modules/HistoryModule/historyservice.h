#ifndef HISTORYSERVICE_H
#define HISTORYSERVICE_H

#include <QObject>

#include "irestservice.h"
#include "iservice.h"
class IProxyConnection;
class IDatabaseUpdateQuery;
class IDatabaseUpdate;
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



   int registerPageQuery(IDatabaseUpdate *update, QString url, QString title);
   void registerVisitQuery(IDatabaseUpdate *update, int user_id, int page_id);
   void registerEdgeQuery(IDatabaseUpdate *update, int page_from_id, int page_to_id);
   void registerTraverseQuery(IDatabaseUpdate *update, int user_id, int page_from_id, int page_to_id);
};


#endif // HISTORYSERVICES_H
