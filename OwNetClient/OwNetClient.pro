#-------------------------------------------------
#
# Project created by QtCreator 2012-09-29T17:32:25
#
#-------------------------------------------------

QT       += core gui network sql qtestlib

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
    helpers/listofstringpairs.cpp \
    proxy/input/proxystaticinputobject.cpp \
    helpers/qjson/json_parser.cc \
    helpers/qjson/json_scanner.cpp \
    helpers/qjson/parser.cpp \
    helpers/qjson/parserrunnable.cpp \
    helpers/qjson/qobjecthelper.cpp \
    helpers/qjson/serializer.cpp \
    helpers/qjson/serializerrunnable.cpp \
    proxy/input/bus/proxyrequestbus.cpp \
    modules/testmodule.cpp \
    modules/imodule.cpp \
    init/moduleinitializer.cpp \
    modules/ibus.cpp \
    init/databaseinitializer.cpp \
    modules/databasemodule.cpp

HEADERS  += view/mainwindow.h \
        proxy/proxyserver.h \
    init/proxyinitializer.h \
    init/initializer.h \
    helpers/messagehelper.h \
    proxy/proxyrequest.h \
    proxy/proxyhandler.h \
    proxy/input/proxyinputobject.h \
    proxy/input/proxywebinputobject.h \
    helpers/listofstringpairs.h \
    proxy/input/proxystaticinputobject.h \
    helpers/qjson/json_parser.hh \
    helpers/qjson/json_scanner.h \
    helpers/qjson/location.hh \
    helpers/qjson/parser_p.h \
    helpers/qjson/parser.h \
    helpers/qjson/parserrunnable.h \
    helpers/qjson/position.hh \
    helpers/qjson/qjson_debug.h \
    helpers/qjson/qobjecthelper.h \
    helpers/qjson/serializer.h \
    helpers/qjson/serializerrunnable.h \
    helpers/qjson/stack.hh \
    proxy/input/bus/proxyrequestbus.h \
    modules/testmodule.h \
    modules/imodule.h \
    init/moduleinitializer.h \
    modules/ibus.h \
    init/databaseinitializer.h \
    modules/databasemodule.h

FORMS    += view/mainwindow.ui

INCLUDEPATH += proxy init helpers view proxy/input proxy/input/bus modules
