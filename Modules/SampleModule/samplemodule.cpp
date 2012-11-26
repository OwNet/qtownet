#include "samplemodule.h"

#include "service.h"

QList<IRestService *> *SampleModule::restServices()
{
    QList<IRestService *> *list = new QList<IRestService *>;
    list->append(new Service(this));
    return list;
}

Q_EXPORT_PLUGIN2(ownet_samplemodule, SampleModule)
