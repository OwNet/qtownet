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
    modules/imodule.cpp \
    init/moduleinitializer.cpp \
    modules/ibus.cpp \
    init/databaseinitializer.cpp \
    modules/databasemodule.cpp \
    proxy/downloads/proxydownloads.cpp \
    proxy/downloads/proxydownload.cpp \
    proxy/output/proxyoutputwriter.cpp \
    proxy/output/proxysocketoutputwriter.cpp \
    helpers/applicationdatastorage.cpp \
    proxy/output/proxycacheoutputwriter.cpp \
    proxy/input/proxycacheinputobject.cpp \
    cache/cachefolder.cpp \
    proxy/downloads/proxydownloadpart.cpp \
    proxy/downloads/proxybytedownloadpart.cpp \
    proxy/downloads/proxystreamdownloadpart.cpp \
    proxy/downloads/proxylastdownloadpart.cpp \
    modules/usermodule.cpp \
    modules/sessionmodule.cpp\
    jobs/job.cpp \
    init/jobinitializer.cpp \
    jobs/cleancachejob.cpp \
    cache/gdsfclock.cpp \
    database/databaseupdatequery.cpp \
    database/databaseupdate.cpp


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
    modules/imodule.h \
    init/moduleinitializer.h \
    modules/ibus.h \
    init/databaseinitializer.h \
    modules/databasemodule.h \
    proxy/downloads/proxydownloads.h \
    proxy/downloads/proxydownload.h \
    proxy/output/proxyoutputwriter.h \
    proxy/output/proxysocketoutputwriter.h \
    helpers/applicationdatastorage.h \
    proxy/output/proxycacheoutputwriter.h \
    proxy/input/proxycacheinputobject.h \
    cache/cachefolder.h \
    proxy/downloads/proxydownloadpart.h \
    proxy/downloads/proxybytedownloadpart.h \
    proxy/downloads/proxystreamdownloadpart.h \
    proxy/downloads/proxylastdownloadpart.h \
    modules/usermodule.h \
    modules/sessionmodule.h\
    jobs/job.h \
    init/jobinitializer.h \
    jobs/cleancachejob.h \
    cache/gdsfclock.h \
    database/databaseupdatequery.h \
    database/databaseupdate.h\

FORMS    += view/mainwindow.ui

INCLUDEPATH += proxy \
    init \
    helpers \
    view \
    proxy/input \
    proxy/input/bus \
    proxy/downloads \
    proxy/output modules \
    jobs \
    cache \
    database
