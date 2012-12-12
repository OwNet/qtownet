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

    QVariant *processRequest(IBus *, IRequest *);

private:

   int m_pageCounter;
   PrefetchingModule* m_module;
   IProxyConnection *m_proxyConnection;


   QByteArray *link(IBus *, IRequest *);
   QByteArray *close(IBus *, IRequest *);
   QByteArray *load(IBus *, IRequest *);
   QByteArray *done(IBus *, IRequest *);


   //int registerPage(QString &url);
};



#endif // SERVICES_H
