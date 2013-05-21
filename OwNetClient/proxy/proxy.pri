INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

SOURCES += $${OwNetClientPath}/proxy/proxyrequest.cpp \
    $${OwNetClientPath}/proxy/proxyhandler.cpp \
    $${OwNetClientPath}/proxy/proxytrafficcounter.cpp \
    $${OwNetClientPath}/proxy/proxyhandlersession.cpp \
    $${OwNetClientPath}/proxy/httpserver/httpserver.cpp \
    $${OwNetClientPath}/proxy/httpserver/sockethandler.cpp \
    $${OwNetClientPath}/proxy/httpserver/requestreader.cpp \
    $${OwNetClientPath}/proxy/staticreader.cpp \
    $${OwNetClientPath}/proxy/servicereader.cpp \
    $${OwNetClientPath}/proxy/websocket.cpp \
    $${OwNetClientPath}/proxy/webdownloadsmanager.cpp \
    $${OwNetClientPath}/proxy/cachelocations.cpp \
    $${OwNetClientPath}/proxy/webreader.cpp \
    $${OwNetClientPath}/proxy/webdownload.cpp \
    $${OwNetClientPath}/proxy/directwebreader.cpp \
    $${OwNetClientPath}/proxy/websocketoutput.cpp \
    proxy/cacheaccessmanager.cpp

HEADERS  += $${OwNetClientPath}/proxy/proxyrequest.h \
    $${OwNetClientPath}/proxy/proxyhandler.h \
    $${OwNetClientPath}/proxy/proxytrafficcounter.h \
    $${OwNetClientPath}/proxy/proxyhandlersession.h \
    $${OwNetClientPath}/proxy/httpserver/httpserver.h \
    $${OwNetClientPath}/proxy/httpserver/sockethandler.h \
    $${OwNetClientPath}/proxy/httpserver/requestreader.h \
    $${OwNetClientPath}/proxy/staticreader.h \
    $${OwNetClientPath}/proxy/servicereader.h \
    $${OwNetClientPath}/proxy/websocket.h \
    $${OwNetClientPath}/proxy/webdownloadsmanager.h \
    $${OwNetClientPath}/proxy/cachelocations.h \
    $${OwNetClientPath}/proxy/webreader.h \
    $${OwNetClientPath}/proxy/webdownload.h \
    $${OwNetClientPath}/proxy/directwebreader.h \
    $${OwNetClientPath}/proxy/iwebdownload.h \
    $${OwNetClientPath}/proxy/websocketoutput.h \
    proxy/cacheaccessmanager.h

INCLUDEPATH += $${OwNetClientPath}/proxy \
    $${OwNetClientPath}/proxy/httpserver
