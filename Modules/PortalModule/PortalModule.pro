QT             -= gui
QT             += sql network

TARGET          = $$qtLibraryTarget(ownet_portalmodule)
TEMPLATE        = lib
CONFIG         += plugin

INCLUDEPATH    += ../../OwNetClient/modules/interfaces

SOURCES        += portalmodule.cpp \
    usersservice.cpp \
    groupsservice.cpp \
    sessionservice.cpp \
    historyservice.cpp \
    messagesservice.cpp \
    ratingsservice.cpp\
    portalhelper.cpp \
    recommendationsservice.cpp \
    activitiesservice.cpp \
    activity.cpp \
    activitymanager.cpp \
    ratingmanager.cpp \
    recommendationmanager.cpp \
    sharedfilesservice.cpp \
    sharedfilesmanager.cpp \
    sharedfilesupdatelistener.cpp

HEADERS        += portalmodule.h \
    usersservice.h \
    groupsservice.h \
    sessionservice.h \
    historyservice.h \
   messagesservice.h \
   ratingsservice.h \
    portalhelper.h \
    recommendationsservice.h \
    activitiesservice.h \
    activity.h \
    activitymanager.h \
    ratingmanager.h \
    recommendationmanager.h \
    sharedfilesservice.h \
    sharedfilesmanager.h \
    sharedfilesupdatelistener.h

macx:DESTDIR    = ../../OwNetClient/OwNetClient.app/Contents/MacOS/modules
!macx:DESTDIR   = ../../OwNetClient/modules

macx:QMAKE_CXXFLAGS += -stdlib=libc++
QMAKE_CXXFLAGS += -std=c++0x
