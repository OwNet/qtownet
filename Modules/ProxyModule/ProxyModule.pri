ModulePath = $${ModulesPath}/ProxyModule

INCLUDEPATH    += $${OwNetClientPath}/modules/interfaces \
    $${ModulePath}

SOURCES        += $${ModulePath}/refreshservice.cpp \
    $${ModulePath}/webpinger.cpp \
    $${ModulePath}/webpingerjob.cpp \
    refreshsession.cpp

HEADERS        += $${ModulePath}/refreshservice.h \
    $${ModulePath}/webpinger.h \
    $${ModulePath}/webpingerjob.h \
    refreshsession.h
