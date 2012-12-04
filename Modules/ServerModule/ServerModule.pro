QT             -= gui
QT             += sql network

TARGET          = $$qtLibraryTarget(ownet_servermodule)
TEMPLATE        = lib
CONFIG         += plugin

INCLUDEPATH    += ../../OwNetClient/modules/interfaces

SOURCES        += servermodule.cpp \
    serverservice.cpp \
    clientservice.cpp \
    clientservicecall.cpp

HEADERS        += servermodule.h \
    serverservice.h \
    clientservice.h \
    clientservicecall.h

TARGET          = $$qtLibraryTarget(ownet_servermodule)
macx:DESTDIR    = ../../OwNetClient/OwNetClient.app/Contents/MacOS/modules
!macx:DESTDIR   = ../../OwNetClient/modules
