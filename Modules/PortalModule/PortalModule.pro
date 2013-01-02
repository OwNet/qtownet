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
    historyservice.cpp \
   # messagesservice.cpp \
   # ratingsservice.cpp
    portalhelper.cpp

HEADERS        += portalmodule.h \
    usersservice.h \
    groupsservice.h \
    sessionservice.h \
    historyservice.h \
   # messagesservice.h \
   # ratingsservice.h \
    portalhelper.h

macx:DESTDIR    = ../../OwNetClient/OwNetClient.app/Contents/MacOS/modules
!macx:DESTDIR   = ../../OwNetClient/modules

macx:QMAKE_CXXFLAGS += -stdlib=libc++
QMAKE_CXXFLAGS += -std=c++0x
