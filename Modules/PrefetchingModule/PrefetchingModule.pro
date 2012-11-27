QT             += gui
QT             += sql
QT             += network
QT             += webkit

TARGET          = $$qtLibraryTarget(ownet_prefetchingmodule)
TEMPLATE        = lib
CONFIG         += plugin

INCLUDEPATH    += ../../OwNetClient/modules/interfaces

SOURCES        += prefetchingmodule.cpp \
    prefetchjob.cpp \
    loggedpage.cpp \
    service.cpp

HEADERS        += prefetchingmodule.h \
    prefetchjob.h \
    loggedpage.h \
    service.h

TARGET          = $$qtLibraryTarget(ownet_prefetchingmodule)
macx:DESTDIR    = ../../OwNetClient/OwNetClient.app/Contents/MacOS/modules
!macx:DESTDIR   = ../../OwNetClient/modules
