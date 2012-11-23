QT             -= gui
QT             += network sql

TARGET          = $$qtLibraryTarget(ownet_sessionmodule)
TEMPLATE        = lib
CONFIG         += plugin

INCLUDEPATH    += ../../OwNetClient

SOURCES        += sessionmodule.cpp

HEADERS        += sessionmodule.h

TARGET          = $$qtLibraryTarget(ownet_sessionmodule)
macx:DESTDIR    = ../../OwNetClient/OwNetClient.app/Contents/MacOS/modules
!macx:DESTDIR   = ../../OwNetClient/modules
