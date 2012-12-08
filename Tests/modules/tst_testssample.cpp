#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QtCore/QCoreApplication>
#include <QDebug>

#include "autotest.h"
#include "modulehelpers.h"

#include "artificialrequest.h"
#include "proxyconnection.h"
#include "irequest.h"
#include "requestrouter.h"
#include "imodule.h"
#include "irestservice.h"

class TestsSample : public QObject
{
    Q_OBJECT

public:
    TestsSample();

private:
    IRestService *m_restService;
    IModule *m_module;    
    ProxyConnection *m_proxyConnection;

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testIndex();
    void testIndex_data();
};

TestsSample::TestsSample()
{
}

void TestsSample::initTestCase()
{
    m_module = ModuleHelpers::loadModule("ownet_samplemodule");

    // initialize module
    m_proxyConnection = new ProxyConnection();
    m_module->init(m_proxyConnection);
    m_restService = RequestRouter::getRestSerivce("sample");
}

void TestsSample::cleanupTestCase()
{
}

void TestsSample::testIndex()
{
    IRequest* req = m_proxyConnection->createRequest(IRequest::GET,"sample");
    IResponse* response = m_restService->index(req);
    QFETCH(QString, module);
    QVariant  body = response->body();
    QCOMPARE(body.toMap().value("module").toString(), module);
}

void TestsSample::testIndex_data()
{
    QTest::addColumn<QString>("module");
    QTest::newRow("name") << QString("sample");
}

DECLARE_TEST(TestsSample)

#include "tst_testssample.moc"
