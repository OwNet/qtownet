QT             -= gui
QT             += sql

TARGET          = $$qtLibraryTarget(ownet_samplemodule)
TEMPLATE        = lib
CONFIG         += plugin

INCLUDEPATH    += ../../OwNetClient/modules/interfaces

SOURCES        += samplemodule.cpp \
    sampleservice.cpp

HEADERS        += samplemodule.h \
    sampleservice.h

TARGET          = $$qtLibraryTarget(ownet_samplemodule)
macx:DESTDIR    = ../../OwNetClient/OwNetClient.app/Contents/MacOS/modules
!macx:DESTDIR   = ../../OwNetClient/modules

macx:QMAKE_CXXFLAGS += -stdlib=libc++
QMAKE_CXXFLAGS += -std=c++11
