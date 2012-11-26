QT             -= gui
QT             += sql

TARGET          = $$qtLibraryTarget(ownet_usersmodule)
TEMPLATE        = lib
CONFIG         += plugin

INCLUDEPATH    += ../../OwNetClient/modules/interfaces

SOURCES        += usersmodule.cpp \
    service.cpp

HEADERS        += usersmodule.h \
    service.h

TARGET          = $$qtLibraryTarget(ownet_usersmodule)
macx:DESTDIR    = ../../OwNetClient/OwNetClient.app/Contents/MacOS/modules
!macx:DESTDIR   = ../../OwNetClient/modules
