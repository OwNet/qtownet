QT             -= gui
QT             += network sql

TARGET          = $$qtLibraryTarget(ownet_sessionmodule)
TEMPLATE        = lib
CONFIG         += plugin

INCLUDEPATH    += ../../OwNetClient/modules/interfaces

SOURCES        += sessionmodule.cpp \
    sessionservice.cpp

HEADERS        += sessionmodule.h \
    sessionservice.h

TARGET          = $$qtLibraryTarget(ownet_sessionmodule)
macx:DESTDIR    = ../../OwNetClient/OwNetClient.app/Contents/MacOS/modules
!macx:DESTDIR   = ../../OwNetClient/modules
