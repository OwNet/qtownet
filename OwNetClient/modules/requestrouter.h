#ifndef REQUESTROUTER_H
#define REQUESTROUTER_H

#include <QObject>

class IRestService;
class IService;
class IBus;
class IRequest;

class RequestRouter : public QObject
{
    Q_OBJECT
public:
    RequestRouter(IService *iService, QObject *parent = 0);
    RequestRouter(IRestService *iRestService, QObject *parent = 0);

    QByteArray *processRequest(IBus *bus, IRequest *req) const;
    QVariant *processRestRequest(IBus *bus, IRequest *req) const;

    QString moduleName() const;
    
private:
    IService *m_iService;
    IRestService *m_iRestService;
};

#endif // REQUESTROUTER_H
