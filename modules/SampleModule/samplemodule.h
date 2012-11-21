#ifndef SAMPLEMODULE_H
#define SAMPLEMODULE_H

#include <QObject>

#include "modules/irestmodule.h"

class SampleModule : public QObject, public IRestModule
{
    Q_OBJECT
    Q_INTERFACES(IRestModule)

public:
    QString name() const { return "sample"; }

    QByteArray *index(IBus *, IRequest *request);
};

#endif // SAMPLEMODULE_H
