QT             -= gui
QT             += network sql

TARGET          = $$qtLibraryTarget(ownet_sessionmodule)
TEMPLATE        = lib
CONFIG         += plugin

INCLUDEPATH    += ../../OwNetClient/modules/interfaces

SOURCES        += sessionmodule.cpp \
    service.cpp

HEADERS        += sessionmodule.h \
    service.h

TARGET          = $$qtLibraryTarget(ownet_sessionmodule)
macx:DESTDIR    = ../../OwNetClient/OwNetClient.app/Contents/MacOS/modules
!macx:DESTDIR   = ../../OwNetClient/modules
