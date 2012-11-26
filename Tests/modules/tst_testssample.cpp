#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QtCore/QCoreApplication>
#include <QDebug>

#include "autotest.h"
#include "stub/stubbus.h"
#include "stub/stubrequest.h"

#include "../OwNetClient/modules/irestmodule.h"

class TestsSample : public QObject
{
    Q_OBJECT

public:
    TestsSample();

private:
    IRestModule *m_restModule;
    StubBus *m_stubBus;
    StubRequest *m_stubRequest;

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
    QPluginLoader loader("/home/martin/School/TP/QtOwNet-build-desktop-Qt_4_8_1_in_PATH__System__Debug/OwNetClient/modules/libownet_samplemodule.so");
    QObject *plugin = loader.instance();
    m_restModule = qobject_cast<IRestModule *>(plugin);

    // initialize stubs
    m_stubBus = new StubBus(this);
    m_stubRequest = new StubRequest(this);
    m_stubRequest->setModule("sample");
}

void TestsSample::cleanupTestCase()
{
}

void TestsSample::testIndex()
{
    QVariantMap response = m_restModule->index(m_stubBus, m_stubRequest)->toMap();
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
