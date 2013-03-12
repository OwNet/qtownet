SOURCES += $${OwNetClientPath}/modules/requestrouter.cpp \
    $${OwNetClientPath}/modules/route.cpp \
    $${OwNetClientPath}/modules/response.cpp


HEADERS  += $${OwNetClientPath}/modules/interfaces/imodule.h \
    $${OwNetClientPath}/modules/interfaces/irequest.h \
    $${OwNetClientPath}/modules/requestrouter.h \
    $${OwNetClientPath}/modules/interfaces/isession.h \
    $${OwNetClientPath}/modules/interfaces/idatabaseupdatequery.h \
    $${OwNetClientPath}/modules/interfaces/ijobaction.h \
    $${OwNetClientPath}/modules/interfaces/iproxyconnection.h \
    $${OwNetClientPath}/modules/interfaces/iservice.h \
    $${OwNetClientPath}/modules/interfaces/irestservice.h \
    $${OwNetClientPath}/modules/interfaces/idatabasesettings.h \
    $${OwNetClientPath}/modules/interfaces/idatabaseselectquery.h \
    $${OwNetClientPath}/modules/interfaces/idatabaseselectqueryjoin.h \
    $${OwNetClientPath}/modules/interfaces/idatabaseselectquerywheregroup.h \
    $${OwNetClientPath}/modules/interfaces/irouter.h \
    $${OwNetClientPath}/modules/route.h \
    $${OwNetClientPath}/modules/interfaces/iroute.h \
    $${OwNetClientPath}/modules/interfaces/iresponse.h \
    $${OwNetClientPath}/modules/response.h \
    modules/interfaces/isynceddatabaseupdatequery.h \
    modules/interfaces/idatabaseupdatelistener.h

INCLUDEPATH += $${OwNetClientPath}/modules \
    $${OwNetClientPath}/modules/interfaces
