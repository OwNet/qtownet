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


SOURCES +=  tst_testssample.cpp \
            stubbus.cpp \
            stubrequest.cpp \
            tst_testscommunicationmanager.cpp \
            ../OwNetClient/communication/communicationmanager.cpp \
            ../OwNetClient/communication/communicationinstance.cpp \
    main.cpp

HEADERS +=  ../OwNetClient/modules/ibus.h \
            ../OwNetClient/modules/irequest.h \
            ../OwNetClient/modules/irestmodule.h \
            ../OwNetClient/modules/isession.h \
            ../OwNetClient/modules/idatabaseupdate.h \
            ../OwNetClient/modules/idatabaseupdatequery.h \
            ../OwNetClient/communication/communicationmanager.h \
            ../OwNetClient/communication/communicationinstance.h \
            stubbus.h \
            stubrequest.h \
    autotest.h

DEFINES += SRCDIR=\\\"$$PWD/\\\"
