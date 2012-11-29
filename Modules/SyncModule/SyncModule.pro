QT             -= gui
QT             += sql

TARGET          = $$qtLibraryTarget(ownet_syncmodule)
TEMPLATE        = lib
CONFIG         += plugin

INCLUDEPATH    += ../../OwNetClient/modules/interfaces

SOURCES        += syncmodule.cpp \
    syncjob.cpp \
    syncserver.cpp

HEADERS        += syncmodule.h \
    syncjob.h \
    syncserver.h

TARGET          = $$qtLibraryTarget(ownet_syncmodule)
macx:DESTDIR    = ../../OwNetClient/OwNetClient.app/Contents/MacOS/modules
!macx:DESTDIR   = ../../OwNetClient/modules
