QT             -= gui
QT             += sql

TARGET          = $$qtLibraryTarget(ownet_messagesmodule)
TEMPLATE        = lib
CONFIG         += plugin

INCLUDEPATH    += ../../OwNetClient/modules/interfaces

SOURCES        += messagesmodule.cpp \
    messagesservice.cpp

HEADERS        += messagesmodule.h \
    messagesservice.h

TARGET          = $$qtLibraryTarget(ownet_messagesmodule)
macx:DESTDIR    = ../../OwNetClient/OwNetClient.app/Contents/MacOS/modules
!macx:DESTDIR   = ../../OwNetClient/modules
