QT             -= gui
QT             += sql

TARGET          = $$qtLibraryTarget(ownet_ratingsmodule)
TEMPLATE        = lib
CONFIG         += plugin

INCLUDEPATH    += ../../OwNetClient/modules/interfaces

SOURCES        += ratingsmodule.cpp \
    ratingsservice.cpp

HEADERS        += ratingsmodule.h \
    ratingsservice.h

TARGET          = $$qtLibraryTarget(ownet_ratingsmodule)
macx:DESTDIR    = ../../OwNetClient/OwNetClient.app/Contents/MacOS/modules
!macx:DESTDIR   = ../../OwNetClient/modules
