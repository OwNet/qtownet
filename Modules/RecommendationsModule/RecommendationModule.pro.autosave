QT             -= gui
QT             += sql

TARGET          = $$qtLibraryTarget(ownet_recommendationsmodule)
TEMPLATE        = lib
CONFIG         += plugin

INCLUDEPATH    += ../../OwNetClient/modules/interfaces

SOURCES        += recommendationsmodule.cpp \
    service.cpp

HEADERS        += recommendationsmodule.h \
    service.h

TARGET          = $$qtLibraryTarget(ownet_recommendationsmodule)
macx:DESTDIR    = ../../OwNetClient/OwNetClient.app/Contents/MacOS/modules
!macx:DESTDIR   = ../../OwNetClient/modules
