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
    proxy/testserver.cpp \
    proxy/testsocket.cpp \
    proxy/proxydownloadtests.cpp \
    proxy/proxyrequesttests.cpp


HEADERS +=  autotest.h \
    stub/stubtime.h \
    helpers/modulehelpers.h \
    stub/stubdatabase.h \
    modules/syncservertests.h \
    proxy/testserver.h \
    proxy/testsocket.h \
    proxy/proxydownloadtests.h \
    proxy/proxyrequesttests.h

OwNetClientPath = ../OwNetClient
ModulesPath = ../Modules

include($${OwNetClientPath}/helpers/helpers.pri)
include($${OwNetClientPath}/modules/modules.pri)
include($${OwNetClientPath}/database/database.pri)
include($${OwNetClientPath}/jobs/jobs.pri)
include($${OwNetClientPath}/cache/cache.pri)
include($${OwNetClientPath}/proxy/proxy.pri)

include($${ModulesPath}/MulticastModule/MulticastModule.pri)
include($${ModulesPath}/SyncModule/SyncModule.pri)

INCLUDEPATH += $$_PRO_FILE_PWD_ \
    stub \
    helpers \
    modules \
    proxy

DEFINES += SRCDIR=\\\"$$_PRO_FILE_PWD_\\\" TEST

macx:QMAKE_CXXFLAGS += -stdlib=libc++
QMAKE_CXXFLAGS += -std=c++0x
