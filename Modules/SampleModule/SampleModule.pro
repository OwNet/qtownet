QT             -= gui
QT             += sql

TARGET          = $$qtLibraryTarget(ownet_samplemodule)
TEMPLATE        = lib
CONFIG         += plugin

INCLUDEPATH    += ../../OwNetClient/modules/interfaces

SOURCES        += samplemodule.cpp \
    service.cpp

HEADERS        += samplemodule.h \
    service.h

TARGET          = $$qtLibraryTarget(ownet_samplemodule)
macx:DESTDIR    = ../../OwNetClient/OwNetClient.app/Contents/MacOS/modules
!macx:DESTDIR   = ../../OwNetClient/modules
