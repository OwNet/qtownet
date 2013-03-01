#-------------------------------------------------
#
# Project created by QtCreator 2013-03-01T08:24:26
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OwNetService
TEMPLATE = app


SOURCES += main.cpp \
    helpers/applicationenvironment.cpp \
    helpers/settings.cpp \
    initializers/settingsinitializer.cpp \
    ownetclient.cpp \
    ownetcloudserver.cpp

HEADERS  += helpers/applicationenvironment.h \
            helpers/settings.h \
    initializers/settingsinitializer.h \
    ownetclient.h \
    ownetcloudserver.h

INCLUDEPATH += helpers initializers
