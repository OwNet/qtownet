#ifndef PREFETCHINGSERVICES_H
#define PREFETCHINGSERVICES_H

#include <QObject>
#include <QList>
#include "iservice.h"

class IProxyConnection;
class PrefetchingModule;
class IDatabaseUpdateQuery;
class IDatabaseUpdate;
class QFile;
class QUrl;

class PrefetchingService : public QObject, public IService
{
    Q_OBJECT
public:
    PrefetchingService(IProxyConnection *proxyConnection, QObject* parent);

    void init(IRouter *router);

    QString name() const { return "prefetch"; }

    IResponse *create(IRequest *);
    IResponse *close(IRequest *);
    IResponse *load(IRequest *);
    IResponse *done(IRequest *);
    IResponse *list(IRequest *);
    IResponse *index(IRequest *);
    IResponse *deleteOrder(IRequest *);
    IResponse *allPagesCount(IRequest *);

private:
    static const int DEFAULT_PRIORITY = 10;
    IProxyConnection *m_proxyConnection;
    void registerPredictionsQuery(uint from, QStringList &url);
    bool completedPrefetchingQuery(QString url);
    bool disablePredictionQuery(uint hash);

    //QStringList getPageLinks(QString url);
    QStringList getCachedLinks(QStringList links);
   //int registerPage(QString &url);
};



#endif // SERVICES_H
