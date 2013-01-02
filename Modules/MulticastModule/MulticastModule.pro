QT             -= gui
QT             += sql network

TARGET          = $$qtLibraryTarget(ownet_servermodule)
TEMPLATE        = lib
CONFIG         += plugin

OwNetClientPath = ../../OwNetClient
ModulesPath = ..

include(MulticastModule.pri)

TARGET          = $$qtLibraryTarget(ownet_multicastmodule)
macx:DESTDIR    = ../../OwNetClient/OwNetClient.app/Contents/MacOS/modules
!macx:DESTDIR   = ../../OwNetClient/modules

macx:QMAKE_CXXFLAGS += -stdlib=libc++
QMAKE_CXXFLAGS += -std=c++0x
