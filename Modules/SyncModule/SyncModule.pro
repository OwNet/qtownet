QT             -= gui
QT             += sql

TARGET          = $$qtLibraryTarget(ownet_syncmodule)
TEMPLATE        = lib
CONFIG         += plugin

INCLUDEPATH    += ../../OwNetClient/modules/interfaces

SOURCES        += syncmodule.cpp

HEADERS        += syncmodule.h

TARGET          = $$qtLibraryTarget(ownet_syncmodule)
macx:DESTDIR    = ../../OwNetClient/OwNetClient.app/Contents/MacOS/modules
!macx:DESTDIR   = ../../OwNetClient/modules
