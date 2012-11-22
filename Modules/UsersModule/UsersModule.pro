QT             -= gui
QT             += sql

TARGET          = $$qtLibraryTarget(ownet_usersmodule)
TEMPLATE        = lib
CONFIG         += plugin

INCLUDEPATH    += ../../OwNetClient

SOURCES        += usersmodule.cpp

HEADERS        += usersmodule.h

TARGET          = $$qtLibraryTarget(ownet_usersmodule)
macx:DESTDIR    = ../../OwNetClient/OwNetClient.app/Contents/MacOS/modules
!macx:DESTDIR   = ../../OwNetClient/modules
