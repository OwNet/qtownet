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
    init/proxyinitializer.cpp \
    init/initializer.cpp \
    helpers/messagehelper.cpp \
    proxy/proxyrequest.cpp \
    proxy/proxyhandler.cpp \
    proxy/input/proxyinputobject.cpp \
    proxy/input/proxywebinputobject.cpp \
    helpers/listofstringpairs.cpp

HEADERS  += view/mainwindow.h \
        proxy/proxyserver.h \
    init/proxyinitializer.h \
    init/initializer.h \
    helpers/messagehelper.h \
    proxy/proxyrequest.h \
    proxy/proxyhandler.h \
    proxy/input/proxyinputobject.h \
    proxy/input/proxywebinputobject.h \
    helpers/listofstringpairs.h

FORMS    += view/mainwindow.ui

INCLUDEPATH += proxy init helpers view proxy/input
