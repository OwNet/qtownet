QT             += gui sql network webkitwidgets

TARGET          = $$qtLibraryTarget(ownet_prefetchingmodule)
TEMPLATE        = lib
CONFIG         += plugin


INCLUDEPATH    += ../../OwNetClient/modules/interfaces

#LibPath = ../../lib

SOURCES        += prefetchingmodule.cpp \
    prefetchingservice.cpp \
    prefetchingjob.cpp \
    browserworker.cpp \
    downloadordersservice.cpp

HEADERS        += prefetchingmodule.h \
    prefetchingservice.h \
    prefetchingjob.h \
    browserworker.h \
    noalertwebpage.h \
    downloadordersservice.h

#include($${LibPath}/QSgml/QSgml.pri)

TARGET          = $$qtLibraryTarget(ownet_prefetchingmodule)
macx:DESTDIR    = ../../OwNetClient/OwNetClient.app/Contents/MacOS/modules
!macx:DESTDIR   = ../../OwNetClient/modules

macx:QMAKE_CXXFLAGS += -stdlib=libc++
QMAKE_CXXFLAGS += -std=c++0x
