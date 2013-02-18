ModulePath = $${ModulesPath}/MulticastModule

INCLUDEPATH    += $${OwNetClientPath}/modules/interfaces \
    $${ModulePath}

SOURCES        += $${ModulePath}/multicastmodule.cpp \
    $${ModulePath}/multicastserver.cpp \
    $${ModulePath}/multicastjob.cpp \
    $${ModulePath}/multicastprotocolnode.cpp \
    $${ModulePath}/multicastprotocol.cpp \
    $${ModulePath}/multicastservice.cpp \
    $${ModulePath}/updatejob.cpp \
    $${ModulePath}/pingserver.cpp \
    $${ModulePath}/pingservice.cpp \
    $${ModulePath}/pingjob.cpp


HEADERS        += $${ModulePath}/multicastmodule.h \
    $${ModulePath}/multicastserver.h \
    $${ModulePath}/multicastjob.h \
    $${ModulePath}/multicastprotocolnode.h \
    $${ModulePath}/multicastprotocol.h \
    $${ModulePath}/multicastservice.h \
    $${ModulePath}/updatejob.h \
    $${ModulePath}/pingserver.h \
    $${ModulePath}/pingservice.h \
    $${ModulePath}/pingjob.h
