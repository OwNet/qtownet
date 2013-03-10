ModulePath = $${ModulesPath}/SyncModule

INCLUDEPATH    += $${OwNetClientPath}/modules/interfaces \
    $${ModulePath} \
    $${ModulePath}/centralservice

SOURCES        += $${ModulePath}/syncjob.cpp \
    $${ModulePath}/syncserver.cpp \
    $${ModulePath}/syncservice.cpp \
    $${ModulePath}/syncclient.cpp \
    $${ModulePath}/synclock.cpp \
    $${ModulePath}/centralservice/centralservicesyncjob.cpp \
    $${ModulePath}/centralservice/centralservicereporter.cpp \
    $${ModulePath}/centralservice/centralserviceupdater.cpp

HEADERS        += $${ModulePath}/syncjob.h \
    $${ModulePath}/syncserver.h \
    $${ModulePath}/syncservice.h \
    $${ModulePath}/syncclient.h \
    $${ModulePath}/synclock.h \
    $${ModulePath}/centralservice/centralservicesyncjob.h \
    $${ModulePath}/centralservice/centralservicereporter.h \
    $${ModulePath}/centralservice/centralserviceupdater.h
