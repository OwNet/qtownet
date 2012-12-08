QT             -= gui
QT             += sql

TARGET          = $$qtLibraryTarget(ownet_portalmodule)
TEMPLATE        = lib
CONFIG         += plugin

INCLUDEPATH    += ../../OwNetClient/modules/interfaces

SOURCES        += portalmodule.cpp \
    usersservice.cpp \
    groupsservice.cpp \
    sessionservice.cpp \
    messagesservice.cpp \
    ratingsservice.cpp

HEADERS        += portalmodule.h \
    usersservice.h \
    groupsservice.h \
    sessionservice.h \
    messagesservice.h \
    ratingsservice.h

TARGET          = $$qtLibraryTarget(ownet_samplemodule)
macx:DESTDIR    = ../../OwNetClient/OwNetClient.app/Contents/MacOS/modules
!macx:DESTDIR   = ../../OwNetClient/modules


QMAKE_CXXFLAGS += -std=c++0x
