#ifndef SERVICES_H
#define SERVICES_H

#include <QObject>

#include "irestservice.h"
#include "iresponse.h"

class IProxyConnection;

class SampleService : public QObject, public IRestService
{
    Q_OBJECT
public:    
    explicit SampleService(IProxyConnection *proxyConnection, QObject *parent = 0);

    QString name() const { return "sample"; }
    IResponse *index(IRequest *req);
};

#endif // SERVICES_H
