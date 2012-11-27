QT             -= gui
QT             += sql

TARGET          = $$qtLibraryTarget(ownet_usersmodule)
TEMPLATE        = lib
CONFIG         += plugin

INCLUDEPATH    += ../../OwNetClient/modules/interfaces

SOURCES        += usersmodule.cpp \
    usersservice.cpp

HEADERS        += usersmodule.h \
    usersservice.h

TARGET          = $$qtLibraryTarget(ownet_usersmodule)
macx:DESTDIR    = ../../OwNetClient/OwNetClient.app/Contents/MacOS/modules
!macx:DESTDIR   = ../../OwNetClient/modules
