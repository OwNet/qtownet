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
    ownet/ownetclient.cpp \
    ownet/ownetcloudserver.cpp \
    helpers/jsondocument/jsondocument.cpp \
    helpers/jsondocument/qjsonwriter.cpp \
    helpers/jsondocument/qjson.cpp \
    helpers/version.cpp \
    ownet/ownetupdater.cpp \
    ownet/ownetinstallation.cpp

HEADERS  += helpers/applicationenvironment.h \
            helpers/settings.h \
    initializers/settingsinitializer.h \
    ownet/ownetclient.h \
    ownet/ownetcloudserver.h \
    helpers/jsondocument/jsondocument.h \
    helpers/jsondocument/qjsonwriter.h \
    helpers/jsondocument/qjson.h \
    helpers/version.h \
    ownet/ownetupdater.h \
    ownet/ownetinstallation.h

INCLUDEPATH += helpers initializers ownet
