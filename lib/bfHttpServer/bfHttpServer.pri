INCLUDEPATH += $${LibPath}/bfHttpServer
DEPENDPATH += $${LibPath}/bfHttpServer

HEADERS += $${LibPath}/bfHttpServer/httplistener.h \
    $${LibPath}/bfHttpServer/httpconnectionhandler.h \
    $${LibPath}/bfHttpServer/httpconnectionhandlerpool.h \
    $${LibPath}/bfHttpServer/httprequest.h \
    $${LibPath}/bfHttpServer/httpresponse.h \
    $${LibPath}/bfHttpServer/httpcookie.h \
    $${LibPath}/bfHttpServer/httprequesthandler.h
HEADERS += $${LibPath}/bfHttpServer/httpsession.h \
    $${LibPath}/bfHttpServer/httpsessionstore.h
HEADERS += $${LibPath}/bfHttpServer/staticfilecontroller.h

SOURCES += $${LibPath}/bfHttpServer/httplistener.cpp \
    $${LibPath}/bfHttpServer/httpconnectionhandler.cpp \
    $${LibPath}/bfHttpServer/httpconnectionhandlerpool.cpp \
    $${LibPath}/bfHttpServer/httprequest.cpp \
    $${LibPath}/bfHttpServer/httpresponse.cpp \
    $${LibPath}/bfHttpServer/httpcookie.cpp \
    $${LibPath}/bfHttpServer/httprequesthandler.cpp
SOURCES += $${LibPath}/bfHttpServer/httpsession.cpp \
    $${LibPath}/bfHttpServer/httpsessionstore.cpp
SOURCES += $${LibPath}/bfHttpServer/staticfilecontroller.cpp
