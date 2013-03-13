ModulePath = $${ModulesPath}/MulticastModule

INCLUDEPATH    += $${OwNetClientPath}/modules/interfaces \
    $${ModulePath} \
    $${ModulePath}/ping

SOURCES        += $${ModulePath}/multicastmodule.cpp \
    $${ModulePath}/multicastserver.cpp \
    $${ModulePath}/multicastjob.cpp \
    $${ModulePath}/multicastprotocolnode.cpp \
    $${ModulePath}/multicastprotocol.cpp \
    $${ModulePath}/multicastservice.cpp \
    $${ModulePath}/updatejob.cpp \
    $${ModulePath}/ping/pingserver.cpp \
    $${ModulePath}/ping/pingservice.cpp \
    $${ModulePath}/ping/pingjob.cpp


HEADERS        += $${ModulePath}/multicastmodule.h \
    $${ModulePath}/multicastserver.h \
    $${ModulePath}/multicastjob.h \
    $${ModulePath}/multicastprotocolnode.h \
    $${ModulePath}/multicastprotocol.h \
    $${ModulePath}/multicastservice.h \
    $${ModulePath}/updatejob.h \
    $${ModulePath}/ping/pingserver.h \
    $${ModulePath}/ping/pingservice.h \
    $${ModulePath}/ping/pingjob.h
