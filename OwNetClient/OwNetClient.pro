#-------------------------------------------------
#
# Project created by QtCreator 2012-09-29T17:32:25
#
#-------------------------------------------------

QT       += core gui network sql qtestlib webkit

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
    modules/prefetching/prefetchingmodule.cpp \
    modules/prefetching/loggedpage.cpp \
    jobs/job.cpp \
    init/jobinitializer.cpp \
    jobs/cleancachejob.cpp \
    cache/gdsfclock.cpp \
    jobs/prefetchjob.cpp \
    helpers/capybarawebkit/WindowFocus.cpp \
    helpers/capybarawebkit/WebPageManager.cpp \
    helpers/capybarawebkit/WebPage.cpp \
    helpers/capybarawebkit/Visit.cpp \
    helpers/capybarawebkit/Url.cpp \
    helpers/capybarawebkit/UnsupportedContentHandler.cpp \
    helpers/capybarawebkit/TimeoutCommand.cpp \
    helpers/capybarawebkit/Status.cpp \
    helpers/capybarawebkit/Source.cpp \
    helpers/capybarawebkit/SocketCommand.cpp \
    helpers/capybarawebkit/SetTimeout.cpp \
    helpers/capybarawebkit/SetSkipImageLoading.cpp \
    helpers/capybarawebkit/SetProxy.cpp \
    helpers/capybarawebkit/SetPromptText.cpp \
    helpers/capybarawebkit/SetPromptAction.cpp \
    helpers/capybarawebkit/SetCookie.cpp \
    helpers/capybarawebkit/SetConfirmAction.cpp \
    helpers/capybarawebkit/Server.cpp \
    helpers/capybarawebkit/Response.cpp \
    helpers/capybarawebkit/ResizeWindow.cpp \
    helpers/capybarawebkit/Reset.cpp \
    helpers/capybarawebkit/RequestedUrl.cpp \
    helpers/capybarawebkit/Render.cpp \
    helpers/capybarawebkit/PageLoadingCommand.cpp \
    helpers/capybarawebkit/NullCommand.cpp \
    helpers/capybarawebkit/Node.cpp \
    helpers/capybarawebkit/NetworkCookieJar.cpp \
    helpers/capybarawebkit/NetworkAccessManager.cpp \
    helpers/capybarawebkit/JavascriptPromptMessages.cpp \
    helpers/capybarawebkit/JavascriptInvocation.cpp \
    helpers/capybarawebkit/JavascriptConfirmMessages.cpp \
    helpers/capybarawebkit/JavascriptAlertMessages.cpp \
    helpers/capybarawebkit/IgnoreSslErrors.cpp \
    helpers/capybarawebkit/Headers.cpp \
    helpers/capybarawebkit/Header.cpp \
    helpers/capybarawebkit/GetWindowHandles.cpp \
    helpers/capybarawebkit/GetWindowHandle.cpp \
    helpers/capybarawebkit/GetTimeout.cpp \
    helpers/capybarawebkit/GetCookies.cpp \
    helpers/capybarawebkit/FrameFocus.cpp \
    helpers/capybarawebkit/Find.cpp \
    helpers/capybarawebkit/Execute.cpp \
    helpers/capybarawebkit/Evaluate.cpp \
    helpers/capybarawebkit/EnableLogging.cpp \
    helpers/capybarawebkit/CurrentUrl.cpp \
    helpers/capybarawebkit/ConsoleMessages.cpp \
    helpers/capybarawebkit/Connection.cpp \
    helpers/capybarawebkit/CommandParser.cpp \
    helpers/capybarawebkit/CommandFactory.cpp \
    helpers/capybarawebkit/Command.cpp \
    helpers/capybarawebkit/ClearPromptText.cpp \
    helpers/capybarawebkit/ClearCookies.cpp \
    helpers/capybarawebkit/body.cpp \
    helpers/capybarawebkit/Authenticate.cpp

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
    modules/prefetching/prefetchingmodule.h \
    modules/prefetching/loggedpage.h \
    jobs/job.h \
    init/jobinitializer.h \
    jobs/cleancachejob.h \
    cache/gdsfclock.h \
    jobs/prefetchjob.h \
    helpers/capybarawebkit/WindowFocus.h \
    helpers/capybarawebkit/WebPageManager.h \
    helpers/capybarawebkit/WebPage.h \
    helpers/capybarawebkit/Visit.h \
    helpers/capybarawebkit/Url.h \
    helpers/capybarawebkit/UnsupportedContentHandler.h \
    helpers/capybarawebkit/TimeoutCommand.h \
    helpers/capybarawebkit/Status.h \
    helpers/capybarawebkit/Source.h \
    helpers/capybarawebkit/SocketCommand.h \
    helpers/capybarawebkit/SetTimeout.h \
    helpers/capybarawebkit/SetSkipImageLoading.h \
    helpers/capybarawebkit/SetProxy.h \
    helpers/capybarawebkit/SetPromptText.h \
    helpers/capybarawebkit/SetPromptAction.h \
    helpers/capybarawebkit/SetCookie.h \
    helpers/capybarawebkit/SetConfirmAction.h \
    helpers/capybarawebkit/Server.h \
    helpers/capybarawebkit/Response.h \
    helpers/capybarawebkit/ResizeWindow.h \
    helpers/capybarawebkit/Reset.h \
    helpers/capybarawebkit/RequestedUrl.h \
    helpers/capybarawebkit/Render.h \
    helpers/capybarawebkit/PageLoadingCommand.h \
    helpers/capybarawebkit/NullCommand.h \
    helpers/capybarawebkit/Node.h \
    helpers/capybarawebkit/NetworkCookieJar.h \
    helpers/capybarawebkit/NetworkAccessManager.h \
    helpers/capybarawebkit/JavascriptPromptMessages.h \
    helpers/capybarawebkit/JavascriptInvocation.h \
    helpers/capybarawebkit/JavascriptConfirmMessages.h \
    helpers/capybarawebkit/JavascriptAlertMessages.h \
    helpers/capybarawebkit/IgnoreSslErrors.h \
    helpers/capybarawebkit/Headers.h \
    helpers/capybarawebkit/Header.h \
    helpers/capybarawebkit/GetWindowHandles.h \
    helpers/capybarawebkit/GetWindowHandle.h \
    helpers/capybarawebkit/GetTimeout.h \
    helpers/capybarawebkit/GetCookies.h \
    helpers/capybarawebkit/FrameFocus.h \
    helpers/capybarawebkit/find_command.h \
    helpers/capybarawebkit/Find.h \
    helpers/capybarawebkit/Execute.h \
    helpers/capybarawebkit/Evaluate.h \
    helpers/capybarawebkit/EnableLogging.h \
    helpers/capybarawebkit/CurrentUrl.h \
    helpers/capybarawebkit/ConsoleMessages.h \
    helpers/capybarawebkit/Connection.h \
    helpers/capybarawebkit/CommandParser.h \
    helpers/capybarawebkit/CommandFactory.h \
    helpers/capybarawebkit/Command.h \
    helpers/capybarawebkit/ClearPromptText.h \
    helpers/capybarawebkit/ClearCookies.h \
    helpers/capybarawebkit/Body.h \
    helpers/capybarawebkit/Authenticate.h

FORMS    += view/mainwindow.ui

INCLUDEPATH += proxy \
    init \
    helpers \
    helpers/qjson \
    helpers/capybarawebkit \
    view \
    proxy/input \
    proxy/input/bus \
    proxy/downloads \
    proxy/output modules \
    jobs \
    cache

OTHER_FILES += \
    helpers/capybarawebkit/capybara.js

RESOURCES += \
    OwNetClient.qrc
