QT             -= gui
QT             += sql network

TARGET          = $$qtLibraryTarget(ownet_servermodule)
TEMPLATE        = lib
CONFIG         += plugin

INCLUDEPATH    += ../../OwNetClient/modules/interfaces

SOURCES        += servermodule.cpp \
    serverservice.cpp \
    clientservice.cpp \
    clientservicecall.cpp \
    customserverservice.cpp \
    centralserviceservice.cpp


HEADERS        += servermodule.h \
    serverservice.h \
    clientservice.h \
    clientservicecall.h \
    customserverservice.h \
    centralserviceservice.h

TARGET          = $$qtLibraryTarget(ownet_servermodule)
macx:DESTDIR    = ../../OwNetClient/OwNetClient.app/Contents/MacOS/modules
!macx:DESTDIR   = ../../OwNetClient/modules

macx:QMAKE_CXXFLAGS += -stdlib=libc++
QMAKE_CXXFLAGS += -std=c++0x
