QT             -= gui
QT             += sql network

TARGET          = $$qtLibraryTarget(ownet_historymodule)
TEMPLATE        = lib
CONFIG         += plugin

INCLUDEPATH    += ../../OwNetClient/modules/interfaces

SOURCES        += historymodule.cpp \
    historyservice.cpp

HEADERS        += historymodule.h \
    historyservice.h

TARGET          = $$qtLibraryTarget(ownet_historymodule)
macx:DESTDIR    = ../../OwNetClient/OwNetClient.app/Contents/MacOS/modules
!macx:DESTDIR   = ../../OwNetClient/modules
