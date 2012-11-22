#include "samplemodule.h"

#include "modules/irequest.h"
#include <QDebug>

QVariant *SampleModule::index(IBus *, IRequest *request)
{
    QVariantMap values;
    values.insert("module", request->module());
    return new QVariant(values);
}

Q_EXPORT_PLUGIN2(ownet_samplemodule, SampleModule)
