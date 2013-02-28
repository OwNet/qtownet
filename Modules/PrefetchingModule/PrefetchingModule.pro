QT             += gui sql network webkitwidgets

TARGET          = $$qtLibraryTarget(ownet_prefetchingmodule)
TEMPLATE        = lib
CONFIG         += plugin


INCLUDEPATH    += ../../OwNetClient/modules/interfaces

LibPath = ../../lib

SOURCES        += prefetchingmodule.cpp \
    loggedpage.cpp \
    prefetchingservice.cpp \
    prefetchingjob.cpp \
    browserworker.cpp

HEADERS        += prefetchingmodule.h \
    loggedpage.h \
    prefetchingservice.h \
    prefetchingjob.h \
    browserworker.h \
    noalertwebpage.h

include($${LibPath}/QSgml/QSgml.pri)

TARGET          = $$qtLibraryTarget(ownet_prefetchingmodule)
macx:DESTDIR    = ../../OwNetClient/OwNetClient.app/Contents/MacOS/modules
!macx:DESTDIR   = ../../OwNetClient/modules
