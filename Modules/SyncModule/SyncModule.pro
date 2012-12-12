QT             -= gui
QT             += sql

TARGET          = $$qtLibraryTarget(ownet_syncmodule)
TEMPLATE        = lib
CONFIG         += plugin

INCLUDEPATH    += ../../OwNetClient/modules/interfaces

SOURCES        += syncmodule.cpp \
    syncjob.cpp \
    syncserver.cpp \
    syncservice.cpp \
    syncclient.cpp \
    synclock.cpp

HEADERS        += syncmodule.h \
    syncjob.h \
    syncserver.h \
    syncservice.h \
    syncclient.h \
    synclock.h

TARGET          = $$qtLibraryTarget(ownet_syncmodule)
macx:DESTDIR    = ../../OwNetClient/OwNetClient.app/Contents/MacOS/modules
!macx:DESTDIR   = ../../OwNetClient/modules

macx:QMAKE_CXXFLAGS += -stdlib=libc++
QMAKE_CXXFLAGS += -std=c++0x
