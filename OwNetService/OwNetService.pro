#-------------------------------------------------
#
# Project created by QtCreator 2013-03-01T08:24:26
#
#-------------------------------------------------

QT       += core gui network

LIBS     += -lz

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
    ownet/ownetinstallation.cpp \
    quazip/zip.c \
    quazip/unzip.c \
    quazip/quazipnewinfo.cpp \
    quazip/quazipfileinfo.cpp \
    quazip/quazipfile.cpp \
    quazip/quazipdir.cpp \
    quazip/quazip.cpp \
    quazip/quaziodevice.cpp \
    quazip/quagzipfile.cpp \
    quazip/quacrc32.cpp \
    quazip/quaadler32.cpp \
    quazip/qioapi.cpp \
    quazip/JlCompress.cpp

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
    ownet/ownetinstallation.h \
    quazip/zip.h \
    quazip/unzip.h \
    quazip/quazipnewinfo.h \
    quazip/quazipfileinfo.h \
    quazip/quazipfile.h \
    quazip/quazipdir.h \
    quazip/quazip.h \
    quazip/quazip_global.h \
    quazip/quaziodevice.h \
    quazip/quagzipfile.h \
    quazip/quacrc32.h \
    quazip/quachecksum32.h \
    quazip/quaadler32.h \
    quazip/JlCompress.h \
    quazip/ioapi.h \
    quazip/crypt.h

INCLUDEPATH += helpers initializers ownet quazip
