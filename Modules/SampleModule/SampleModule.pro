QT             -= gui
QT             += sql

TARGET          = $$qtLibraryTarget(ownet_samplemodule)
TEMPLATE        = lib
CONFIG         += plugin

INCLUDEPATH    += ../../OwNetClient

SOURCES        += samplemodule.cpp

HEADERS        += samplemodule.h

TARGET          = $$qtLibraryTarget(ownet_samplemodule)
macx:DESTDIR    = ../../OwNetClient/OwNetClient.app/Contents/MacOS/modules
!macx:DESTDIR   = ../../OwNetClient/modules
