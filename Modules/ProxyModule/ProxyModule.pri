ModulePath = $${ModulesPath}/ProxyModule

INCLUDEPATH    += $${OwNetClientPath}/modules/interfaces \
    $${ModulePath}

SOURCES        += $${ModulePath}/refreshservice.cpp \
    $${ModulePath}/webpinger.cpp \
    $${ModulePath}/webpingerjob.cpp \
    $${ModulePath}/refreshsession.cpp \
    $${ModulePath}/cacheexceptionsmanager.cpp \
    $${ModulePath}/cacheexceptionsservice.cpp

HEADERS        += $${ModulePath}/refreshservice.h \
    $${ModulePath}/webpinger.h \
    $${ModulePath}/webpingerjob.h \
    $${ModulePath}/refreshsession.h \
    $${ModulePath}/cacheexceptionsmanager.h \
    $${ModulePath}/cacheexceptionsservice.h
