#ifndef SERVICES_H
#define SERVICES_H

#include <QObject>

#include "irestservice.h"

class Service : public QObject, public IRestService
{
    Q_OBJECT
public:
    Service(QObject *parent = NULL);

    QString name() const { return "sample"; }
    QVariant *index(IBus *, IRequest *request);
};

#endif // SERVICES_H
