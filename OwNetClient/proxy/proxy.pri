INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

SOURCES += $${OwNetClientPath}/proxy/proxyserver.cpp \
    $${OwNetClientPath}/proxy/proxyrequest.cpp \
    $${OwNetClientPath}/proxy/proxyhandler.cpp \
    $${OwNetClientPath}/proxy/input/proxyinputobject.cpp \
    $${OwNetClientPath}/proxy/input/proxywebinputobject.cpp \
    $${OwNetClientPath}/proxy/input/proxystaticinputobject.cpp \
    $${OwNetClientPath}/proxy/input/proxyrequestbus.cpp \
    $${OwNetClientPath}/proxy/downloads/proxydownloads.cpp \
    $${OwNetClientPath}/proxy/downloads/proxydownload.cpp \
    $${OwNetClientPath}/proxy/output/proxyoutputwriter.cpp \
    $${OwNetClientPath}/proxy/output/proxysocketoutputwriter.cpp \
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
    $${OwNetClientPath}/proxy/output/proxysocket.cpp


HEADERS  += $${OwNetClientPath}/proxy/proxyserver.h \
    $${OwNetClientPath}/proxy/proxyrequest.h \
    $${OwNetClientPath}/proxy/proxyhandler.h \
    $${OwNetClientPath}/proxy/input/proxyinputobject.h \
    $${OwNetClientPath}/proxy/input/proxywebinputobject.h \
    $${OwNetClientPath}/proxy/input/proxystaticinputobject.h \
    $${OwNetClientPath}/proxy/input/proxyrequestbus.h \
    $${OwNetClientPath}/proxy/downloads/proxydownloads.h \
    $${OwNetClientPath}/proxy/downloads/proxydownload.h \
    $${OwNetClientPath}/proxy/output/proxyoutputwriter.h \
    $${OwNetClientPath}/proxy/output/proxysocketoutputwriter.h \
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
    $${OwNetClientPath}/proxy/output/isocket.h \
    $${OwNetClientPath}/proxy/output/proxysocket.h

INCLUDEPATH += $${OwNetClientPath}/proxy \
    $${OwNetClientPath}/proxy/downloads \
    $${OwNetClientPath}/proxy/output \
    $${OwNetClientPath}/proxy/input
