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
    proxy/input/proxyrequestbus.cpp \
    init/moduleinitializer.cpp \
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
    jobs/job.cpp \
    init/jobinitializer.cpp \
    jobs/cleancachejob.cpp \
    cache/gdsfclock.cpp \
    database/databaseupdatequery.cpp \
    database/databaseupdate.cpp \
    helpers/applicationenvironment.cpp \
    helpers/applicationproxyfactory.cpp \
    proxy/proxytrafficcounter.cpp \
    jobs/proxytrafficcounterjob.cpp \
    proxy/proxyhandlersession.cpp \
    helpers/settings.cpp \
    view/preferencesdialog.cpp \
    init/settingsinitializer.cpp \
    proxy/downloads/proxycachefiledownloadpart.cpp \
    helpers/variantmap.cpp \
    modules/requestrouter.cpp \
    helpers/session.cpp

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
    proxy/input/proxyrequestbus.h \
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
    jobs/job.h \
    init/jobinitializer.h \
    jobs/cleancachejob.h \
    cache/gdsfclock.h \
    database/databaseupdatequery.h \
    database/databaseupdate.h \
    helpers/applicationenvironment.h \
    helpers/applicationproxyfactory.h \
    proxy/proxytrafficcounter.h \
    jobs/proxytrafficcounterjob.h \
    proxy/proxyhandlersession.h \
    helpers/settings.h \
    view/preferencesdialog.h \
    init/settingsinitializer.h \
    proxy/downloads/proxycachefiledownloadpart.h \
    modules/irequest.h \
    helpers/variantmap.h \
    modules/irestmodule.h \
    modules/requestrouter.h \
    modules/isession.h \
    helpers/session.h \
    modules/idatabaseupdate.h \
    modules/idatabaseupdatequery.h

FORMS    += view/mainwindow.ui \
    view/preferencesdialog.ui

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

RESOURCES += \
    resources.qrc
