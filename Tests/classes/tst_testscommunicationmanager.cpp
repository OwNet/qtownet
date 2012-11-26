#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QtCore/QCoreApplication>
#include <QDebug>

#include "autotest.h"
#include "stub/stubtime.h"

#include "../OwNetClient/communication/communicationmanager.h"
#include "../OwNetClient/communication/communicationinstance.h"

class TestsCommunicationManager : public QObject
{
    Q_OBJECT

public:
    static const int period;
    static const int second;

    TestsCommunicationManager();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    void testGetInstance();

    void testGetCommunicationInstancesWhenCreated();
    void testGetCommunicationInstancesWhenOneHearbeatIsReceived();
    void testGetCommunicationInstancesWhenMultipleHearbeatsAreReceived();
    void testGetCommunicationInstancesWhenOneHearbeatWithChangedInfoAreReceived();
    void testGetCommunicationInstancesWhenMultipleHearbeatsWithChangedInfoAreReceived();
    void testGetCommunicationInstancesWhenNoHearbeatsAreReceived();

    void testGetServerWhenCreated();
    void testGetServerWhenOneHeartbeatIsReceived();
    void testGetServerWhenMultipleHeartbeatsAreReceived();
    void testGetServerWhenInitializing();
    void testGetServerWhenNoHeartbeatsAreReceived();

    void testMyStatusInitializing();
    void testMyStatusClient();
    void testMyStatusServer();
    void testMyStatusServerAfterCreated();
    void testMyStatusServerWhenNoHeartbeatsAreReceived();
};

const int TestsCommunicationManager::period = 5;
const int TestsCommunicationManager::second = 1;

TestsCommunicationManager::TestsCommunicationManager()
{
}

void TestsCommunicationManager::initTestCase()
{
}

void TestsCommunicationManager::cleanupTestCase()
{
}

void TestsCommunicationManager::testGetInstance()
{
    QVERIFY(CommunicationManager::getInstance() != NULL);
}

void TestsCommunicationManager::testGetCommunicationInstancesWhenCreated()
{
    CommunicationManager *communicationManager = new CommunicationManager;
    CommunicationInstance *instance;

    QVERIFY(communicationManager->getCommunicationInstances().count() == 1);

    instance = communicationManager->getCommunicationInstances().first();
    QCOMPARE(instance->id(), communicationManager->myId());
    QCOMPARE(instance->score(), communicationManager->myScore());
}

void TestsCommunicationManager::testGetCommunicationInstancesWhenOneHearbeatIsReceived()
{
    CommunicationManager *communicationManager = new CommunicationManager;

    QVariantMap message;
    message.insert("id", "unique-client-id-1");
    message.insert("score", "1");
    communicationManager->processMessage(&message);

    QCOMPARE(communicationManager->getCommunicationInstances().count(), 2);

    CommunicationInstance *instance = communicationManager->getCommunicationInstances().last();

    QCOMPARE(instance->id(), QString("unique-client-id-1"));
    QCOMPARE(instance->score(), 1);
}

void TestsCommunicationManager::testGetCommunicationInstancesWhenMultipleHearbeatsAreReceived()
{
    CommunicationManager *communicationManager = new CommunicationManager;

    QVariantMap message1;
    message1.insert("id", "unique-client-id-1");
    message1.insert("score", "1");
    communicationManager->processMessage(&message1);

    QVariantMap message2;
    message2.insert("id", "unique-client-id-2");
    message2.insert("score", "2");
    communicationManager->processMessage(&message2);

    QVariantMap message3;
    message3.insert("id", "unique-client-id-3");
    message3.insert("score", "3");
    communicationManager->processMessage(&message3);

    QCOMPARE(communicationManager->getCommunicationInstances().count(), 4);

    CommunicationInstance *instance;

    instance = communicationManager->getCommunicationInstances().at(0);
    QCOMPARE(instance->id(), QString("unique-client-id-3"));
    QCOMPARE(instance->score(), 3);

    instance = communicationManager->getCommunicationInstances().at(1);
    QCOMPARE(instance->id(), QString("unique-client-id-2"));
    QCOMPARE(instance->score(), 2);
}

void TestsCommunicationManager::testGetCommunicationInstancesWhenOneHearbeatWithChangedInfoAreReceived()
{
    CommunicationManager *communicationManager = new CommunicationManager;

    QVariantMap message1;
    message1.insert("id", "unique-client-id-1");
    message1.insert("score", "1");
    communicationManager->processMessage(&message1);

    QVariantMap message3;
    message3.insert("id", "unique-client-id-1");
    message3.insert("score", "3");
    communicationManager->processMessage(&message3);

    QCOMPARE(communicationManager->getCommunicationInstances().count(), 2);

    CommunicationInstance *instance;

    instance = communicationManager->getCommunicationInstances().at(0);
    QCOMPARE(instance->id(), QString("unique-client-id-1"));
    QCOMPARE(instance->score(), 3);
}

void TestsCommunicationManager::testGetCommunicationInstancesWhenMultipleHearbeatsWithChangedInfoAreReceived()
{
    CommunicationManager *communicationManager = new CommunicationManager;
    CommunicationInstance *instance;

    QVariantMap message1;
    message1.insert("id", "unique-client-id-1");
    message1.insert("score", "1");
    communicationManager->processMessage(&message1);

    QVariantMap message2;
    message2.insert("id", "unique-client-id-2");
    message2.insert("score", "2");
    communicationManager->processMessage(&message2);

    QCOMPARE(communicationManager->getCommunicationInstances().count(), 3);

    instance = communicationManager->getCommunicationInstances().at(0);
    QCOMPARE(instance->id(), QString("unique-client-id-2"));
    QCOMPARE(instance->score(), 2);

    QVariantMap message3;
    message3.insert("id", "unique-client-id-1");
    message3.insert("score", "3");
    communicationManager->processMessage(&message3);

    QCOMPARE(communicationManager->getCommunicationInstances().count(), 3);

    instance = communicationManager->getCommunicationInstances().at(0);
    QCOMPARE(instance->id(), QString("unique-client-id-1"));
    QCOMPARE(instance->score(), 3);

    instance = communicationManager->getCommunicationInstances().at(1);
    QCOMPARE(instance->id(), QString("unique-client-id-2"));
    QCOMPARE(instance->score(), 2);
}

void TestsCommunicationManager::testGetCommunicationInstancesWhenNoHearbeatsAreReceived()
{
    StubTime::setCurrentDateTime(QDateTime::currentDateTime());

    CommunicationManager *communicationManager = new CommunicationManager;
    CommunicationInstance *instance;

    QVariantMap message1;
    message1.insert("id", "unique-client-id-1");
    message1.insert("score", "1");
    communicationManager->processMessage(&message1);

    QVariantMap message2;
    message2.insert("id", "unique-client-id-2");
    message2.insert("score", "2");
    communicationManager->processMessage(&message2);

    QCOMPARE(communicationManager->getCommunicationInstances().count(), 3);

    // QTest::qSleep(2 * period);
    StubTime::addSecs(2 * period);

    QCOMPARE(communicationManager->getCommunicationInstances().count(), 3);

    QVariantMap message3;
    message3.insert("id", "unique-client-id-1");
    message3.insert("score", "1");
    communicationManager->processMessage(&message3);

    //QTest::qSleep(1 * period);
    StubTime::addSecs(1 * period);

    QCOMPARE(communicationManager->getCommunicationInstances().count(), 1);

    //QTest::qSleep(1 * period + 1 * second);
    StubTime::addSecs(1 * period + 1 * second);

    QCOMPARE(communicationManager->getCommunicationInstances().count(), 0);

    QVariantMap message4;
    message4.insert("id", "unique-client-id-1");
    message4.insert("score", "1");
    communicationManager->processMessage(&message4);

    QCOMPARE(communicationManager->getCommunicationInstances().count(), 1);

    StubTime::cleanCurrentDateTime();
}

void TestsCommunicationManager::testGetServerWhenCreated()
{
    CommunicationManager *communicationManager = new CommunicationManager;

    QVERIFY(communicationManager->getServer() != NULL);
    QVERIFY(communicationManager->getServer()->id() == communicationManager->myId());
}

void TestsCommunicationManager::testGetServerWhenOneHeartbeatIsReceived()
{
    CommunicationManager *communicationManager = new CommunicationManager;

    QVERIFY(communicationManager->getServer() != NULL);

    QCOMPARE(communicationManager->getServer()->id(), communicationManager->myId());
}

void TestsCommunicationManager::testGetServerWhenMultipleHeartbeatsAreReceived()
{
    CommunicationManager *communicationManager = new CommunicationManager;

    QVariantMap message2;
    message2.insert("id", "unique-client-id-2");
    message2.insert("score", "2");
    communicationManager->processMessage(&message2);

    QVariantMap message3;
    message3.insert("id", "unique-client-id-3");
    message3.insert("score", "0");
    communicationManager->processMessage(&message3);

    QVERIFY(communicationManager->getServer() != NULL);

    QCOMPARE(communicationManager->getServer()->id(), QString("unique-client-id-2"));
}

void TestsCommunicationManager::testGetServerWhenInitializing()
{
    CommunicationManager *communicationManager = new CommunicationManager;

    QVariantMap message2;
    message2.insert("id", "unique-client-id-2");
    message2.insert("score", "2");
    communicationManager->processMessage(&message2);

    QVERIFY(communicationManager->getServer() != NULL);

    QVariantMap message3;
    message3.insert("id", "unique-client-id-3");
    message3.insert("score", "0");
    message3.insert("status", "initializing");
    communicationManager->processMessage(&message3);

    QVERIFY(communicationManager->getServer() != NULL);

    QVariantMap message5;
    message5.insert("id", "unique-client-id-3");
    message5.insert("score", "5");
    message5.insert("status", "initializing");
    communicationManager->processMessage(&message5);

    QVERIFY(communicationManager->getServer() == NULL);

    QVariantMap message4;
    message4.insert("id", "unique-client-id-3");
    message4.insert("score", "5");
    message4.insert("status", "server");
    communicationManager->processMessage(&message4);

    QVERIFY(communicationManager->getServer() != NULL);

    QCOMPARE(communicationManager->getServer()->id(), QString("unique-client-id-3"));
}

void TestsCommunicationManager::testGetServerWhenNoHeartbeatsAreReceived()
{
    StubTime::setCurrentDateTime(QDateTime::currentDateTime());
    CommunicationManager *communicationManager = new CommunicationManager;

    QVariantMap message1;
    message1.insert("id", "unique-client-id-1");
    message1.insert("score", "5");
    communicationManager->processMessage(&message1);

    QVERIFY(communicationManager->getServer() != NULL);
    QCOMPARE(communicationManager->getServer()->id(), QString("unique-client-id-1"));

    //QTest::qSleep(3 * period + 1 * second);
    StubTime::addSecs(2 * period);

    QVERIFY(communicationManager->getServer() == NULL);

    QVariantMap message2;
    message2.insert("id", "unique-client-id-1");
    message2.insert("score", "1");
    communicationManager->processMessage(&message2);

    QVERIFY(communicationManager->getServer() != NULL);

    StubTime::cleanCurrentDateTime();
}

void TestsCommunicationManager::testMyStatusInitializing()
{
    CommunicationManager *communicationManager = new CommunicationManager;

    QCOMPARE(communicationManager->myStatus(), CommunicationManager::INITIALIZING);
}

void TestsCommunicationManager::testMyStatusClient()
{
    CommunicationManager *communicationManager = new CommunicationManager;

    QVariantMap message;
    message.insert("id", "unique-client-id-stronger");
    message.insert("score", "100");
    message.insert("status", "server");
    communicationManager->processMessage(&message);

    QCOMPARE(communicationManager->myStatus(), CommunicationManager::INITIALIZING);

    communicationManager->initialized();
    QCOMPARE(communicationManager->myStatus(), CommunicationManager::CLIENT);
}

void TestsCommunicationManager::testMyStatusServer()
{
    CommunicationManager *communicationManager = new CommunicationManager;

    QVariantMap message2;
    message2.insert("id", "unique-client-id-weaker-1");
    message2.insert("score", "0");
    communicationManager->processMessage(&message2);

    QVariantMap message3;
    message3.insert("id", "unique-client-id-weaker-2");
    message3.insert("score", "0");
    communicationManager->processMessage(&message3);

    QCOMPARE(communicationManager->myStatus(), CommunicationManager::INITIALIZING);

    communicationManager->initialized();
    QCOMPARE(communicationManager->myStatus(), CommunicationManager::SERVER);
}

void TestsCommunicationManager::testMyStatusServerAfterCreated()
{
    CommunicationManager *communicationManager = new CommunicationManager;

    QCOMPARE(communicationManager->myStatus(), CommunicationManager::INITIALIZING);

    communicationManager->initialized();
    QCOMPARE(communicationManager->myStatus(), CommunicationManager::SERVER);
}

void TestsCommunicationManager::testMyStatusServerWhenNoHeartbeatsAreReceived()
{
    StubTime::setCurrentDateTime(QDateTime::currentDateTime());

    CommunicationManager *communicationManager = new CommunicationManager;

    communicationManager->initialized();

    QVariantMap message2;
    message2.insert("id", "unique-client-id-stronger");
    message2.insert("score", "5");
    communicationManager->processMessage(&message2);

    QVariantMap message3;
    message3.insert("id", "unique-client-id-weaker");
    message3.insert("score", "0");
    communicationManager->processMessage(&message3);

    QCOMPARE(communicationManager->myStatus(), CommunicationManager::CLIENT);

    //QTest::qSleep(1 * period + 1 * second);
    StubTime::addSecs(2 * period);

    // first loopback message
    QVariantMap message1;
    message1.insert("id", communicationManager->myId());
    message1.insert("score", communicationManager->myScore());
    communicationManager->processMessage(&message1);

    QVariantMap message4;
    message4.insert("id", "unique-client-id-weaker");
    message4.insert("score", "0");
    communicationManager->processMessage(&message4);

    QCOMPARE(communicationManager->myStatus(), CommunicationManager::CLIENT);

    //QTest::qSleep(2 * period + 1 * second);
    StubTime::addSecs(2 * period);

    QCOMPARE(communicationManager->myStatus(), CommunicationManager::SERVER);

    StubTime::cleanCurrentDateTime();
}

DECLARE_TEST(TestsCommunicationManager)

#include "tst_testscommunicationmanager.moc"
