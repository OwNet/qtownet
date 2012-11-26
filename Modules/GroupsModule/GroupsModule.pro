QT             -= gui
QT             += sql

TARGET          = $$qtLibraryTarget(ownet_groupsmodule)
TEMPLATE        = lib
CONFIG         += plugin

INCLUDEPATH    += ../../OwNetClient/modules/interfaces

SOURCES        += groupsmodule.cpp \
    groupsservice.cpp

HEADERS        += groupsmodule.h \
    groupsservice.h

TARGET          = $$qtLibraryTarget(ownet_groupsmodule)
macx:DESTDIR    = ../../OwNetClient/OwNetClient.app/Contents/MacOS/modules
!macx:DESTDIR   = ../../OwNetClient/modules
