#-------------------------------------------------
#
# Project created by QtCreator 2012-11-22T12:34:19
#
#-------------------------------------------------

QT       += testlib sql

QT       -= gui

TARGET = tst_testssample
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES +=  ../OwNetClient/helpers/messagehelper.cpp \
            ../OwNetClient/helpers/listofstringpairs.cpp \            
            ../OwNetClient/helpers/applicationdatastorage.cpp \
            ../OwNetClient/jobs/job.cpp \
            ../OwNetClient/database/databaseupdatequery.cpp \
            ../OwNetClient/database/databaseupdate.cpp \
            ../OwNetClient/helpers/applicationenvironment.cpp \
            ../OwNetClient/helpers/settings.cpp \
            ../OwNetClient/helpers/variantmap.cpp \
            ../OwNetClient/modules/requestrouter.cpp \
            ../OwNetClient/helpers/session.cpp \
            ../OwNetClient/helpers/proxyconnection.cpp \
            ../OwNetClient/jobs/modulejob.cpp \
            ../OwNetClient/helpers/artificialrequest.cpp \
            ../OwNetClient/helpers/databasesettings.cpp \
            ../OwNetClient/database/databaseselectquery.cpp \
            ../OwNetClient/database/databaseselectqueryjoin.cpp \
            ../OwNetClient/database/databaseselectquerywheregroup.cpp \
            ../OwNetClient/database/databaseselectquerywhereexpression.cpp \
            ../OwNetClient/modules/route.cpp \
            ../OwNetClient/helpers/jsondocument/jsondocument.cpp \
            ../OwNetClient/helpers/jsondocument/qjsonwriter.cpp \
            ../OwNetClient/helpers/jsondocument/qjson.cpp \
            ../OwNetClient/modules/response.cpp \
            ../Modules/MulticastModule/multicastprotocol.cpp \
            ../Modules/MulticastModule/multicastprotocolnode.cpp \
    main.cpp \
    stub/stubtime.cpp \
    helpers/modulehelpers.cpp \
    stub/stubdatabase.cpp \
    classes/multicastprotocoltests.cpp \
    modules/usersservicetests.cpp \
    modules/sampleservicetests.cpp


HEADERS +=  ../OwNetClient/helpers/messagehelper.h \
            ../OwNetClient/helpers/listofstringpairs.h \                        
            ../OwNetClient/modules/interfaces/imodule.h \
            ../OwNetClient/modules/databasemodule.h \
            ../OwNetClient/helpers/applicationdatastorage.h \
            ../OwNetClient/jobs/job.h \
            ../OwNetClient/jobs/cleancachejob.h \
            ../OwNetClient/database/databaseupdatequery.h \
            ../OwNetClient/database/databaseupdate.h \
            ../OwNetClient/helpers/applicationenvironment.h \
            ../OwNetClient/helpers/applicationproxyfactory.h \
            ../OwNetClient/jobs/proxytrafficcounterjob.h \
            ../OwNetClient/helpers/settings.h \
            ../OwNetClient/modules/interfaces/irequest.h \
            ../OwNetClient/helpers/variantmap.h \
            ../OwNetClient/modules/requestrouter.h \
            ../OwNetClient/modules/interfaces/isession.h \
            ../OwNetClient/helpers/session.h \
            ../OwNetClient/modules/interfaces/idatabaseupdate.h \
            ../OwNetClient/modules/interfaces/idatabaseupdatequery.h \
            ../OwNetClient/modules/interfaces/ijobaction.h \
            ../OwNetClient/modules/interfaces/iproxyconnection.h \
            ../OwNetClient/helpers/proxyconnection.h \
            ../OwNetClient/modules/interfaces/iservice.h \
            ../OwNetClient/modules/interfaces/irestservice.h \
            ../OwNetClient/jobs/modulejob.h \
            ../OwNetClient/helpers/artificialrequest.h \
            ../OwNetClient/helpers/databasesettings.h \
            ../OwNetClient/modules/interfaces/idatabasesettings.h \
            ../OwNetClient/database/databaseselectquery.h \
            ../OwNetClient/database/databaseselectqueryjoin.h \
            ../OwNetClient/modules/interfaces/idatabaseselectquery.h \
            ../OwNetClient/modules/interfaces/idatabaseselectqueryjoin.h \
            ../OwNetClient/database/databaseselectquerywheregroup.h \
            ../OwNetClient/database/databaseselectquerywhereexpression.h \
            ../OwNetClient/database/idatabaseselectquerywhere.h \
            ../OwNetClient/modules/interfaces/idatabaseselectquerywheregroup.h \
            ../OwNetClient/modules/interfaces/irouter.h \
            ../OwNetClient/modules/route.h \
            ../OwNetClient/helpers/jsondocument/jsondocument.h \
            ../OwNetClient/helpers/jsondocument/qjson_p.h \
            ../OwNetClient/modules/interfaces/iroute.h \
            ../OwNetClient/modules/interfaces/iresponse.h \
            ../OwNetClient/modules/response.h \
            ../Modules/MulticastModule/multicastprotocol.h \
            ../Modules/MulticastModule/multicastprotocolnode.h \
    autotest.h \
    stub/stubtime.h \
    helpers/modulehelpers.h \
    stub/stubdatabase.h


INCLUDEPATH += $$_PRO_FILE_PWD_ \
    stub \
    helpers \
    ../OwNetClient/database \
    ../OwNetClient/helpers \
    ../OwNetClient/helpers/jsondocument \
    ../OwNetClient/init \
    ../OwNetClient/modules \
    ../OwNetClient/modules/interfaces \
    ../OwNetClient/jobs



DEFINES += SRCDIR=\\\"$$_PRO_FILE_PWD_\\\" TEST

macx:QMAKE_CXXFLAGS += -stdlib=libc++
QMAKE_CXXFLAGS += -std=c++0x
