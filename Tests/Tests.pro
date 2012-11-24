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


SOURCES += tst_testssample.cpp \
    stubbus.cpp \
    stubrequest.cpp

HEADERS +=  ../OwNetClient/modules/ibus.h \
            ../OwNetClient/modules/irequest.h \
            ../OwNetClient/modules/irestmodule.h \
            ../OwNetClient/modules/isession.h \
            ../OwNetClient/modules/idatabaseupdate.h \
            ../OwNetClient/modules/idatabaseupdatequery.h \
    stubbus.h \
    stubrequest.h

DEFINES += SRCDIR=\\\"$$PWD/\\\"
