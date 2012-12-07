#ifndef SERVICES_H
#define SERVICES_H

#include <QObject>

#include "irestservice.h"

class IProxyConnection;

class SampleService : public QObject, public IRestService
{
    Q_OBJECT
public:    
    explicit SampleService(IProxyConnection *proxyConnection, QObject *parent = 0);

    QString name() const { return "sample"; }
    QVariant *index(IBus *, IRequest *request);
};

#endif // SERVICES_H
