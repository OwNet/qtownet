SOURCES += $${OwNetClientPath}/helpers/messagehelper.cpp \
    $${OwNetClientPath}/helpers/listofstringpairs.cpp \
    $${OwNetClientPath}/helpers/applicationdatastorage.cpp \
    $${OwNetClientPath}/helpers/applicationenvironment.cpp \
    $${OwNetClientPath}/helpers/applicationproxyfactory.cpp \
    $${OwNetClientPath}/helpers/settings.cpp \
    $${OwNetClientPath}/helpers/variantmap.cpp \
    $${OwNetClientPath}/helpers/session.cpp \
    $${OwNetClientPath}/helpers/proxyconnection.cpp \
    $${OwNetClientPath}/helpers/artificialrequest.cpp \
    $${OwNetClientPath}/helpers/databasesettings.cpp \
    $${OwNetClientPath}/helpers/jsondocument/jsondocument.cpp \
    $${OwNetClientPath}/helpers/jsondocument/qjsonwriter.cpp \
    $${OwNetClientPath}/helpers/jsondocument/qjson.cpp \
    $${OwNetClientPath}/helpers/uniqueidhelper.cpp \
    ../OwNetClient/helpers/workspacehelper.cpp


HEADERS  += $${OwNetClientPath}/helpers/messagehelper.h \
    $${OwNetClientPath}/helpers/listofstringpairs.h \
    $${OwNetClientPath}/helpers/applicationdatastorage.h \
    $${OwNetClientPath}/helpers/applicationenvironment.h \
    $${OwNetClientPath}/helpers/applicationproxyfactory.h \
    $${OwNetClientPath}/helpers/settings.h \
    $${OwNetClientPath}/helpers/variantmap.h \
    $${OwNetClientPath}/helpers/session.h \
    $${OwNetClientPath}/helpers/proxyconnection.h \
    $${OwNetClientPath}/helpers/artificialrequest.h \
    $${OwNetClientPath}/helpers/databasesettings.h \
    $${OwNetClientPath}/helpers/jsondocument/jsondocument.h \
    $${OwNetClientPath}/helpers/jsondocument/qjson_p.h \
    $${OwNetClientPath}/helpers/uniqueidhelper.h \
    ../OwNetClient/helpers/workspacehelper.h

INCLUDEPATH += $${OwNetClientPath}/helpers \
    $${OwNetClientPath}/helpers/jsondocument
