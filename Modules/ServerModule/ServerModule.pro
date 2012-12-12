QT             -= gui
QT             += sql network

TARGET          = $$qtLibraryTarget(ownet_servermodule)
TEMPLATE        = lib
CONFIG         += plugin

INCLUDEPATH    += ../../OwNetClient/modules/interfaces \
    helpers/jsondocument

SOURCES        += servermodule.cpp \
    serverservice.cpp \
    clientservice.cpp \
    clientservicecall.cpp \
    multicastserver.cpp \
    multicastjob.cpp \
    helpers/jsondocument/jsondocument.cpp \
    helpers/jsondocument/qjsonwriter.cpp \
    helpers/jsondocument/qjson.cpp \
    multicastprotocolnode.cpp \
    multicastprotocol.cpp


HEADERS        += servermodule.h \
    serverservice.h \
    clientservice.h \
    clientservicecall.h \
    multicastserver.h \
    multicastjob.h \
    helpers/jsondocument/jsondocument.h \
    helpers/jsondocument/qjsonwriter_p.h \
    helpers/jsondocument/qjson_p.h \
    multicastprotocolnode.h \
    multicastprotocol.h

TARGET          = $$qtLibraryTarget(ownet_servermodule)
macx:DESTDIR    = ../../OwNetClient/OwNetClient.app/Contents/MacOS/modules
!macx:DESTDIR   = ../../OwNetClient/modules

macx:QMAKE_CXXFLAGS += -stdlib=libc++
QMAKE_CXXFLAGS += -std=c++0x
