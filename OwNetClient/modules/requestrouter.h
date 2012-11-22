#ifndef REQUESTROUTER_H
#define REQUESTROUTER_H

#include <QObject>

class IModule;
class IRestModule;
class IBus;
class IRequest;

class RequestRouter : public QObject
{
    Q_OBJECT
public:
    RequestRouter(IModule *iModule, QObject *parent = 0);
    RequestRouter(IRestModule *iRestModule, QObject *parent = 0);

    QByteArray *processRequest(IBus *bus, IRequest *req);

    QString moduleName() const;
    
private:
    IModule *m_iModule;
    IRestModule *m_iRestModule;
};

#endif // REQUESTROUTER_H
