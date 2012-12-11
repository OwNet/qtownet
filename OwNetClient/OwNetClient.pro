#-------------------------------------------------
#
# Project created by QtCreator 2012-09-29T17:32:25
#
#-------------------------------------------------

QT       += core gui network sql testlib

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
    proxy/input/proxyrequestbus.cpp \
    init/moduleinitializer.cpp \
    init/databaseinitializer.cpp \
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
    helpers/session.cpp \
    jobs/heartbeatjob.cpp \
    init/communicationinitializer.cpp \
    communication/heartbeatserver.cpp \
    communication/communicationmanager.cpp \
    communication/communicationinstance.cpp \
    helpers/proxyconnection.cpp \
    jobs/modulejob.cpp \
    helpers/artificialrequest.cpp \
    helpers/databasesettings.cpp \
    proxy/downloads/proxydownloadstream.cpp \
    database/databaseselectquery.cpp \
    database/databaseselectqueryjoin.cpp \
    database/databaseselectquerywheregroup.cpp \
    database/databaseselectquerywhereexpression.cpp \
    modules/route.cpp \
    helpers/jsondocument/jsondocument.cpp \
    helpers/jsondocument/qjsonwriter.cpp \
    helpers/jsondocument/qjson.cpp \
    modules/response.cpp \
    jobs/updaterecordssavejob.cpp


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
    proxy/input/proxyrequestbus.h \
    modules/interfaces/imodule.h \
    init/moduleinitializer.h \
    init/databaseinitializer.h \
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
    modules/interfaces/irequest.h \
    helpers/variantmap.h \
    modules/requestrouter.h \
    modules/interfaces/isession.h \
    helpers/session.h \
    jobs/heartbeatjob.h \
    init/communicationinitializer.h \
    communication/heartbeatserver.h \
    communication/communicationmanager.h \
    communication/communicationinstance.h \
    modules/interfaces/idatabaseupdate.h \
    modules/interfaces/idatabaseupdatequery.h \
    modules/interfaces/ijobaction.h \
    modules/interfaces/iproxyconnection.h \
    helpers/proxyconnection.h \
    modules/interfaces/iservice.h \
    modules/interfaces/irestservice.h \
    jobs/modulejob.h \
    helpers/artificialrequest.h \
    helpers/databasesettings.h \
    modules/interfaces/idatabasesettings.h \
    proxy/downloads/proxydownloadstream.h \
    database/databaseselectquery.h \
    database/databaseselectqueryjoin.h \
    modules/interfaces/idatabaseselectquery.h \
    modules/interfaces/idatabaseselectqueryjoin.h \
    database/databaseselectquerywheregroup.h \
    database/databaseselectquerywhereexpression.h \
    database/idatabaseselectquerywhere.h \
    modules/interfaces/idatabaseselectquerywheregroup.h \
    modules/interfaces/irouter.h \
    modules/route.h \
    helpers/jsondocument/jsondocument.h \
    helpers/jsondocument/qjson_p.h \
    modules/interfaces/iroute.h \
    modules/interfaces/iresponse.h \
    modules/response.h \
    jobs/updaterecordssavejob.h


FORMS    += view/mainwindow.ui \
    view/preferencesdialog.ui

INCLUDEPATH += $$_PRO_FILE_PWD_ \
    proxy \
    init \
    helpers \
    helpers/jsondocument \
    view \
    proxy/input \
    proxy/input/bus \
    proxy/downloads \
    proxy/output \
    modules \
    modules/interfaces \
    jobs \
    cache \
    database


RESOURCES += \
    resources.qrc

macx:QMAKE_CXXFLAGS += -stdlib=libc++
QMAKE_CXXFLAGS += -std=c++0x
