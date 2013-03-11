#-------------------------------------------------
#
# Project created by QtCreator 2012-11-22T12:34:19
#
#-------------------------------------------------

QT       += testlib sql network

QT       -= gui

TARGET = tst_testssample
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES +=  main.cpp \
    stub/stubtime.cpp \
    helpers/modulehelpers.cpp \
    stub/stubdatabase.cpp \
    classes/multicastprotocoltests.cpp \
    modules/usersservicetests.cpp \
    modules/sampleservicetests.cpp \
    modules/syncservertests.cpp \
    classes/proxydownloadtests.cpp \
    classes/databaseupdatequerytests.cpp \
    classes/workspaceinitializertests.cpp \
    helpers/workspacehelpertests.cpp \
    classes/databaseselectquerytests.cpp \
    stub/stubdatabaseupdatelistener.cpp


HEADERS +=  autotest.h \
    stub/stubtime.h \
    helpers/modulehelpers.h \
    stub/stubdatabase.h \
    modules/syncservertests.h \
    classes/proxydownloadtests.h \
    classes/databaseupdatequerytests.h \
    classes/workspaceinitializertests.h \
    helpers/workspacehelpertests.h \
    classes/databaseselectquerytests.h \
    stub/stubdatabaseupdatelistener.h

OwNetClientPath = ../OwNetClient
ModulesPath = ../Modules
LibPath = ../lib

include($${OwNetClientPath}/helpers/helpers.pri)
include($${OwNetClientPath}/modules/modules.pri)
include($${OwNetClientPath}/database/database.pri)
include($${OwNetClientPath}/jobs/jobs.pri)
include($${OwNetClientPath}/cache/cache.pri)
include($${OwNetClientPath}/proxy/proxy.pri)
include($${OwNetClientPath}/init/init.pri)
include($${LibPath}/bfHttpServer/bfHttpServer.pri)

include($${ModulesPath}/MulticastModule/MulticastModule.pri)
include($${ModulesPath}/SyncModule/SyncModule.pri)

INCLUDEPATH += $$_PRO_FILE_PWD_ \
    stub \
    helpers \
    modules \
    proxy \
    classes

DEFINES += SRCDIR=\\\"$$_PRO_FILE_PWD_\\\" TEST

macx:QMAKE_CXXFLAGS += -stdlib=libc++
QMAKE_CXXFLAGS += -std=c++0x
