QT             -= gui
QT             += sql network

TARGET          = $$qtLibraryTarget(ownet_servermodule)
TEMPLATE        = lib
CONFIG         += plugin

INCLUDEPATH    += ../../OwNetClient/modules/interfaces \
    helpers/jsondocument

SOURCES        += multicastmodule.cpp \
    multicastserver.cpp \
    multicastjob.cpp \
    multicastprotocolnode.cpp \
    multicastprotocol.cpp \
    multicastservice.cpp \
    updatejob.cpp


HEADERS        += multicastmodule.h \
    multicastserver.h \
    multicastjob.h \
    multicastprotocolnode.h \
    multicastprotocol.h \
    multicastservice.h \
    updatejob.h

TARGET          = $$qtLibraryTarget(ownet_multicastmodule)
macx:DESTDIR    = ../../OwNetClient/OwNetClient.app/Contents/MacOS/modules
!macx:DESTDIR   = ../../OwNetClient/modules

macx:QMAKE_CXXFLAGS += -stdlib=libc++
QMAKE_CXXFLAGS += -std=c++0x
