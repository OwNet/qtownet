INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

SOURCES += $${OwNetClientPath}/proxy/proxyrequest.cpp \
    $${OwNetClientPath}/proxy/proxyhandler.cpp \
    $${OwNetClientPath}/proxy/input/proxyinputobject.cpp \
    $${OwNetClientPath}/proxy/input/proxywebinputobject.cpp \
    $${OwNetClientPath}/proxy/downloads/proxydownloads.cpp \
    $${OwNetClientPath}/proxy/downloads/proxydownload.cpp \
    $${OwNetClientPath}/proxy/output/proxyoutputwriter.cpp \
    $${OwNetClientPath}/proxy/output/proxyresponseoutputwriter.cpp \
    $${OwNetClientPath}/proxy/output/proxycacheoutputwriter.cpp \
    $${OwNetClientPath}/proxy/input/proxycacheinputobject.cpp \
    $${OwNetClientPath}/proxy/downloads/proxydownloadpart.cpp \
    $${OwNetClientPath}/proxy/downloads/proxybytedownloadpart.cpp \
    $${OwNetClientPath}/proxy/downloads/proxystreamdownloadpart.cpp \
    $${OwNetClientPath}/proxy/downloads/proxylastdownloadpart.cpp \
    $${OwNetClientPath}/proxy/proxytrafficcounter.cpp \
    $${OwNetClientPath}/proxy/proxyhandlersession.cpp \
    $${OwNetClientPath}/proxy/downloads/proxycachefiledownloadpart.cpp \
    $${OwNetClientPath}/proxy/downloads/proxydownloadstream.cpp \
    $${OwNetClientPath}/proxy/downloads/proxycachelocations.cpp \
    $${OwNetClientPath}/proxy/httpserver/httpserver.cpp \
    $${OwNetClientPath}/proxy/httpserver/sockethandler.cpp \
    $${OwNetClientPath}/proxy/httpserver/requestreader.cpp \
    $${OwNetClientPath}/proxy/proxystaticreader.cpp \
    $${OwNetClientPath}/proxy/proxyservicereader.cpp \
    proxy/proxywebreader.cpp \
    proxy/proxywebdownload.cpp \
    proxy/websocket.cpp

HEADERS  += $${OwNetClientPath}/proxy/proxyrequest.h \
    $${OwNetClientPath}/proxy/proxyhandler.h \
    $${OwNetClientPath}/proxy/input/proxyinputobject.h \
    $${OwNetClientPath}/proxy/input/proxywebinputobject.h \
    $${OwNetClientPath}/proxy/downloads/proxydownloads.h \
    $${OwNetClientPath}/proxy/downloads/proxydownload.h \
    $${OwNetClientPath}/proxy/output/proxyoutputwriter.h \
    $${OwNetClientPath}/proxy/output/proxyresponseoutputwriter.h \
    $${OwNetClientPath}/proxy/output/proxycacheoutputwriter.h \
    $${OwNetClientPath}/proxy/input/proxycacheinputobject.h \
    $${OwNetClientPath}/proxy/downloads/proxydownloadpart.h \
    $${OwNetClientPath}/proxy/downloads/proxybytedownloadpart.h \
    $${OwNetClientPath}/proxy/downloads/proxystreamdownloadpart.h \
    $${OwNetClientPath}/proxy/downloads/proxylastdownloadpart.h \
    $${OwNetClientPath}/proxy/proxytrafficcounter.h \
    $${OwNetClientPath}/proxy/proxyhandlersession.h \
    $${OwNetClientPath}/proxy/downloads/proxycachefiledownloadpart.h \
    $${OwNetClientPath}/proxy/downloads/proxydownloadstream.h \
    $${OwNetClientPath}/proxy/downloads/proxycachelocations.h \
    $${OwNetClientPath}/proxy/httpserver/httpserver.h \
    $${OwNetClientPath}/proxy/httpserver/sockethandler.h \
    $${OwNetClientPath}/proxy/httpserver/requestreader.h \
    $${OwNetClientPath}/proxy/proxystaticreader.h \
    $${OwNetClientPath}/proxy/proxyservicereader.h \
    proxy/proxywebreader.h \
    proxy/proxywebdownload.h \
    proxy/websocket.h

INCLUDEPATH += $${OwNetClientPath}/proxy \
    $${OwNetClientPath}/proxy/downloads \
    $${OwNetClientPath}/proxy/output \
    $${OwNetClientPath}/proxy/input \
    $${OwNetClientPath}/proxy/httpserver
