QT             -= gui
QT             += sql

TARGET          = $$qtLibraryTarget(ownet_recommendationsmodule)
TEMPLATE        = lib
CONFIG         += plugin

INCLUDEPATH    += ../../OwNetClient/modules/interfaces

SOURCES        += recommendationsmodule.cpp \
    recommendationsservice.cpp

HEADERS        += recommendationsmodule.h \
    recommendationsservice.h

TARGET          = $$qtLibraryTarget(ownet_recommendationsmodule)
macx:DESTDIR    = ../../OwNetClient/OwNetClient.app/Contents/MacOS/modules
!macx:DESTDIR   = ../../OwNetClient/modules
