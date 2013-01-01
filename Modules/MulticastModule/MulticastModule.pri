MulticastModulePath = $${ModulesPath}/MulticastModule

INCLUDEPATH    += $${OwNetClientPath}/modules/interfaces

SOURCES        += $${MulticastModulePath}/multicastmodule.cpp \
    $${MulticastModulePath}/multicastserver.cpp \
    $${MulticastModulePath}/multicastjob.cpp \
    $${MulticastModulePath}/multicastprotocolnode.cpp \
    $${MulticastModulePath}/multicastprotocol.cpp \
    $${MulticastModulePath}/multicastservice.cpp \
    $${MulticastModulePath}/updatejob.cpp


HEADERS        += $${MulticastModulePath}/multicastmodule.h \
    $${MulticastModulePath}/multicastserver.h \
    $${MulticastModulePath}/multicastjob.h \
    $${MulticastModulePath}/multicastprotocolnode.h \
    $${MulticastModulePath}/multicastprotocol.h \
    $${MulticastModulePath}/multicastservice.h \
    $${MulticastModulePath}/updatejob.h
