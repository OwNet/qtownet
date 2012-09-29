#-------------------------------------------------
#
# Project created by QtCreator 2012-09-29T17:32:25
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OwNetClient
TEMPLATE = app


SOURCES += main.cpp\
        view/mainwindow.cpp \
        proxy/proxyserver.cpp \
    proxy/proxythread.cpp \
    init/proxyinitializer.cpp \
    init/initializer.cpp \
    helpers/messagehelper.cpp

HEADERS  += view/mainwindow.h \
        proxy/proxyserver.h \
    proxy/proxythread.h \
    init/proxyinitializer.h \
    init/initializer.h \
    helpers/messagehelper.h

FORMS    += view/mainwindow.ui

INCLUDEPATH += proxy init helpers view
