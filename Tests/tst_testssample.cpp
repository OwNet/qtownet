#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QtCore/QCoreApplication>
#include <QDebug>

#include "../OwNetClient/modules/irestmodule.h"
#include "stubbus.h"
#include "stubrequest.h"

class TestsSample : public QObject
{
    Q_OBJECT
    
public:
    TestsSample();

private:
    IRestModule *m_restModule;
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testIndex();
    void testCase1_data();
};

TestsSample::TestsSample()
{
}

void TestsSample::initTestCase()
{
    QPluginLoader loader("/home/martin/School/TP/QtOwNet-build-desktop-Qt_4_8_1_in_PATH__System__Release/OwNetClient/modules/libownet_samplemodule.so");
    QObject *plugin = loader.instance();
    m_restModule = qobject_cast<IRestModule *>(plugin);
}

void TestsSample::cleanupTestCase()
{
}

void TestsSample::testIndex()
{
    StubBus *stubBus = new StubBus(this);
    StubRequest *stubRequest = new StubRequest(this);
    stubRequest->setModule("sample");
    QVariantMap response = m_restModule->index(stubBus, stubRequest)->toMap();
    //QFETCH(QString, data);
    QCOMPARE(response.value("module").toString(), QString("sample"));
}

void TestsSample::testCase1_data()
{
    QTest::addColumn<QString>("data");
    QTest::newRow("0") << QString();
}

QTEST_MAIN(TestsSample)

#include "tst_testssample.moc"
