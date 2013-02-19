QT             -= gui
QT             += sql

TARGET          = $$qtLibraryTarget(ownet_syncmodule)
TEMPLATE        = lib
CONFIG         += plugin

OwNetClientPath = ../../OwNetClient
ModulesPath = ..

SOURCES += syncmodule.cpp
HEADERS += syncmodule.h

include(SyncModule.pri)

TARGET          = $$qtLibraryTarget(ownet_syncmodule)
macx:DESTDIR    = ../../OwNetClient/OwNetClient.app/Contents/MacOS/modules
!macx:DESTDIR   = ../../OwNetClient/modules

macx:QMAKE_CXXFLAGS += -stdlib=libc++
QMAKE_CXXFLAGS += -std=c++0x
