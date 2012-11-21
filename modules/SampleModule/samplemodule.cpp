#include "samplemodule.h"

QByteArray *SampleModule::index(IBus *, IRequest *request)
{
    return new QByteArray(request->module().toUtf8());
}

Q_EXPORT_PLUGIN2(ownet_samplemodule, SampleModule)
