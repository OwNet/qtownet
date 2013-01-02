ModulePath = $${ModulesPath}/SyncModule

INCLUDEPATH    += $${OwNetClientPath}/modules/interfaces \
    $${ModulePath}

SOURCES        += $${ModulePath}/syncmodule.cpp \
    $${ModulePath}/syncjob.cpp \
    $${ModulePath}/syncserver.cpp \
    $${ModulePath}/syncservice.cpp \
    $${ModulePath}/syncclient.cpp \
    $${ModulePath}/synclock.cpp

HEADERS        += $${ModulePath}/syncmodule.h \
    $${ModulePath}/syncjob.h \
    $${ModulePath}/syncserver.h \
    $${ModulePath}/syncservice.h \
    $${ModulePath}/syncclient.h \
    $${ModulePath}/synclock.h
