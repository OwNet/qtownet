#-------------------------------------------------
#
# Project created by QtCreator 2012-11-22T12:34:19
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_testssample
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES +=  modules/tst_testssample.cpp \
            stub/stubbus.cpp \
            stub/stubrequest.cpp \
            classes/tst_testscommunicationmanager.cpp \
            ../OwNetClient/communication/communicationmanager.cpp \
            ../OwNetClient/communication/communicationinstance.cpp \
    main.cpp \
    stub/stubtime.cpp

HEADERS +=  ../OwNetClient/modules/ibus.h \
            ../OwNetClient/modules/irequest.h \
            ../OwNetClient/modules/irestmodule.h \
            ../OwNetClient/modules/isession.h \
            ../OwNetClient/modules/idatabaseupdate.h \
            ../OwNetClient/modules/idatabaseupdatequery.h \
            ../OwNetClient/communication/communicationmanager.h \
            ../OwNetClient/communication/communicationinstance.h \
            stub/stubbus.h \
            stub/stubrequest.h \
    autotest.h \
    stub/stubtime.h


DEFINES += SRCDIR=\\\"$$PWD/\\\" TEST
