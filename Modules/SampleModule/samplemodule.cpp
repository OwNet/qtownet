#include "samplemodule.h"

#include "sampleservice.h"

QList<IRestService *> *SampleModule::restServices()
{
    QList<IRestService *> *list = new QList<IRestService *>;
    list->append(new SampleService(this));
    return list;
}

