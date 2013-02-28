#ifndef PREFETCHINGSERVICES_H
#define PREFETCHINGSERVICES_H

#include <QObject>

#include "irestservice.h"
#include "iservice.h"
#include "irouter.h"
#include "iresponse.h"

class IProxyConnection;
class PrefetchingModule;
class IDatabaseUpdateQuery;
class IDatabaseUpdate;
class QFile;

class PrefetchingService : public QObject, public IService
{
    Q_OBJECT
public:
    PrefetchingService(IProxyConnection *proxyConnection, QObject* parent);

    void init(IRouter *router);

    QString name() const { return "prefetch"; }

    IResponse *link(IRequest *);
    IResponse *close(IRequest *);
    IResponse *load(IRequest *);
    IResponse *done(IRequest *);

private:
    static const int DEFAULT_PRIORITY = 30;
    IProxyConnection *m_proxyConnection;
    void registerPredictionQuery(int from, QString url);
    bool completedPrefetchingQuery(QString url);
    bool disablePredictionQuery(int hash);


//    QFile* readCache(QUrl url);

   //int registerPage(QString &url);
};



#endif // SERVICES_H
