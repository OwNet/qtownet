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
    stub/stubtime.cpp \
    stub/stubconnection.cpp

HEADERS +=  ../OwNetClient/modules/interfaces/ibus.h \
            ../OwNetClient/modules/interfaces/irequest.h \
            ../OwNetClient/modules/interfaces/irestservice.h \
            ../OwNetClient/modules/interfaces/isession.h \
            ../OwNetClient/modules/interfaces/idatabaseupdate.h \
            ../OwNetClient/modules/interfaces/idatabaseupdatequery.h \
            ../OwNetClient/modules/interfaces/imodule.h \
            ../OwNetClient/modules/interfaces/iproxyconnection.h \
            ../OwNetClient/communication/communicationmanager.h \
            ../OwNetClient/communication/communicationinstance.h \
            stub/stubbus.h \
            stub/stubrequest.h \
    autotest.h \
    stub/stubtime.h \
    stub/stubconnection.h

INCLUDEPATH += $$_PRO_FILE_PWD_


DEFINES += SRCDIR=\\\"$$_PRO_FILE_PWD_\\\" TEST
