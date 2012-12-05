#ifndef SERVICES_H
#define SERVICES_H

#include <QObject>

#include "irestservice.h"
#include "iservice.h"
class IProxyConnection;
class PrefetchingModule;
class IDatabaseUpdateQuery;
class IDatabaseUpdate;

class Service : public QObject, public IService
{
    Q_OBJECT
public:
    Service(IProxyConnection *proxyConnection, PrefetchingModule* pm);

    QString name() const { return "prefetch"; }
//    QVariant *index(IBus *, IRequest *request);

    QByteArray *processRequest(IBus *, IRequest *);

private:

   int m_pageCounter;
   PrefetchingModule* m_module;
   IProxyConnection *m_proxyConnection;

   QByteArray *visit(IBus *, IRequest *);
   QByteArray *link(IBus *, IRequest *);
   QByteArray *close(IBus *, IRequest *);
   QByteArray *load(IBus *, IRequest *);
   QByteArray *done(IBus *, IRequest *);


   int registerPageQuery(IDatabaseUpdate *update, QString url, QString title);
   void registerVisitQuery(IDatabaseUpdate *update, int user_id, int page_id);
   void registerEdgeQuery(IDatabaseUpdate *update, int page_from_id, int page_to_id);
   void registerTraverseQuery(IDatabaseUpdate *update, int user_id, int page_from_id, int page_to_id);

   //int registerPage(QString &url);
};



#endif // SERVICES_H
