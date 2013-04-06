QT             += sql network

TARGET          = $$qtLibraryTarget(ownet_proxymodule)
TEMPLATE        = lib
CONFIG         += plugin

OwNetClientPath = ../../OwNetClient
ModulesPath = ..

SOURCES += proxymodule.cpp
HEADERS += proxymodule.h

include(ProxyModule.pri)

TARGET          = $$qtLibraryTarget(ownet_proxymodule)
macx:DESTDIR    = ../../OwNetClient/OwNetClient.app/Contents/MacOS/modules
!macx:DESTDIR   = ../../OwNetClient/modules

macx:QMAKE_CXXFLAGS += -stdlib=libc++
QMAKE_CXXFLAGS += -std=c++0x
