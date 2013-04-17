#-------------------------------------------------
#
# Project created by QtCreator 2012-09-29T17:32:25
#
#-------------------------------------------------

QT       += core gui network sql testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OwNetClient
TEMPLATE = app

SOURCES += main.cpp \
    ownetapplication.cpp

INCLUDEPATH += $$_PRO_FILE_PWD_


RESOURCES += \
    resources.qrc

OwNetClientPath = .
LibPath = ../lib

include(cache/cache.pri)
include(database/database.pri)
include(helpers/helpers.pri)
include(modules/modules.pri)
include(init/init.pri)
include(jobs/jobs.pri)
include(proxy/proxy.pri)
include(view/view.pri)

macx:QMAKE_CXXFLAGS += -stdlib=libc++
QMAKE_CXXFLAGS += -std=c++0x

HEADERS += \
    ownetapplication.h
