#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QtCore/QCoreApplication>
#include <QDebug>

#include "autotest.h"
#include "stub/stubbus.h"
#include "stub/stubrequest.h"
#include "stub/stubconnection.h"

#include "../OwNetClient/modules/interfaces/imodule.h"
#include "../OwNetClient/modules/interfaces/irestservice.h"

class TestsSample : public QObject
{
    Q_OBJECT

public:
    TestsSample();

private:
    IRestService *m_restService;
    IModule *m_module;
    StubBus *m_stubBus;
    StubRequest *m_stubRequest;
    StubConnection *m_proxyConnection;

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
    // load plugin
    QDir modulesDir = QDir(qApp->applicationDirPath());

#if defined(Q_OS_WIN)
    if (modulesDir.dirName().toLower() == "debug" || modulesDir.dirName().toLower() == "release")
        modulesDir.cdUp();
#endif

    modulesDir.cd("../OwNetClient/modules");

    QPluginLoader loader(modulesDir.absoluteFilePath("libownet_samplemodule.so"));
    QObject *plugin = loader.instance();
    m_module = qobject_cast<IModule *>(plugin);

    // initialize stubs
    m_stubBus = new StubBus(this);
    m_stubRequest = new StubRequest(this);
    m_stubRequest->setModule("sample");

    // initialize module
    m_proxyConnection = new StubConnection();
    m_module->init(m_proxyConnection);
    m_restService = m_proxyConnection->getRestService("sample");
}

void TestsSample::cleanupTestCase()
{
}

void TestsSample::testIndex()
{
    QVariantMap response = m_restService->index(m_stubBus, m_stubRequest)->toMap();
    QFETCH(QString, module);
    QCOMPARE(response.value("module").toString(), module);
}

void TestsSample::testIndex_data()
{
    QTest::addColumn<QString>("module");
    QTest::newRow("name") << QString("sample");
}

DECLARE_TEST(TestsSample)

#include "tst_testssample.moc"
