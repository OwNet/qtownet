#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QtCore/QCoreApplication>
#include <QDebug>

#include "autotest.h"
#include "stub/stubtime.h"
#include "stub/stubdatabase.h"
#include "proxyconnection.h"
#include "session.h"
#include "databasesettings.h"

#include "../Modules/MulticastModule/multicastprotocol.h"
#include "../Modules/MulticastModule/multicastprotocolnode.h"

class MulticastProtocolTests : public QObject
{
    Q_OBJECT

public:
    static const int period;
    static const int second;

    MulticastProtocolTests();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    void testFirstNode();
    void testWeakerNode();
    void testStrongerNode();

private:
    ProxyConnection *m_proxyConnection;
};

const int MulticastProtocolTests::period = 5;
const int MulticastProtocolTests::second = 1;

MulticastProtocolTests::MulticastProtocolTests()
{
}

void MulticastProtocolTests::initTestCase()
{
    m_proxyConnection = new ProxyConnection();
}

void MulticastProtocolTests::cleanupTestCase()
{
}

void MulticastProtocolTests::testFirstNode()
{
    StubTime::setCurrentDateTime(QDateTime::currentDateTime());

    StubDatabase::init();
    DatabaseSettings databaseSettings;
    databaseSettings.createClientId();

    MulticastProtocol protocol(m_proxyConnection, this);
    MulticastProtocolNode *node;
    QVariantMap message;

    QCOMPARE(protocol.nodeList().count(), 1);
    QCOMPARE(protocol.nodeMap().count(), 1);

    // node list
    node = protocol.nodeList().first();
    QCOMPARE(node->id(), databaseSettings.clientId());
    QCOMPARE(node->score(), protocol.currentNode()->score());
    QCOMPARE(node->initialized(), (uint) 0);
    QCOMPARE(node->status(), MulticastProtocol::INITIALIZING);
    QCOMPARE(node->port(), (uint) 8081);

    // node map
    QVERIFY(node == protocol.nodeMap().value(protocol.currentNode()->id()));

    // status
    node->status() == MulticastProtocol::INITIALIZING;

    // message
    message = protocol.message();
    QCOMPARE(message.value("id").toUInt(), databaseSettings.clientId());
    QCOMPARE(message.value("score").toUInt(), protocol.currentNode()->score());
    QCOMPARE(message.value("port").toInt(), 8081);
    QCOMPARE(message.value("initialized").toUInt(), (uint) 0);
    QCOMPARE(message.value("status").toString(), QString("initializing"));

    // wait!
    StubTime::addSecs(5 * period);

    QCOMPARE(protocol.nodeList().count(), 1);
    QCOMPARE(protocol.nodeMap().count(), 1);

    // status
    node->status() == MulticastProtocol::INITIALIZING;

    // message
    message = protocol.message();
    QCOMPARE(message.value("id").toUInt(), databaseSettings.clientId());
    QCOMPARE(message.value("score").toUInt(), protocol.currentNode()->score());
    QCOMPARE(message.value("port").toInt(), 8081);
    QCOMPARE(message.value("initialized").toUInt(), (uint) 0);
    QCOMPARE(message.value("status").toString(), QString("initializing"));

    // initialized!
    protocol.initialized();

    QCOMPARE(protocol.nodeList().count(), 1);
    QCOMPARE(protocol.nodeMap().count(), 1);

    // status
    node->status() == MulticastProtocol::SERVER;

    // message
    message = protocol.message();
    QCOMPARE(message.value("id").toUInt(), databaseSettings.clientId());
    QCOMPARE(message.value("score").toUInt(), protocol.currentNode()->score());
    QCOMPARE(message.value("port").toInt(), 8081);
    QVERIFY(message.value("initialized").toUInt() > (uint) 0);
    QCOMPARE(message.value("status").toString(), QString("server"));

    // wait!
    StubTime::addSecs(5 * period);

    QCOMPARE(protocol.nodeList().count(), 1);
    QCOMPARE(protocol.nodeMap().count(), 1);

    // status
    node->status() == MulticastProtocol::SERVER;

    // message
    message = protocol.message();
    QCOMPARE(message.value("id").toUInt(), databaseSettings.clientId());
    QCOMPARE(message.value("score").toUInt(), protocol.currentNode()->score());
    QCOMPARE(message.value("port").toInt(), 8081);
    QVERIFY(message.value("initialized").toUInt() > (uint) 0);
    QCOMPARE(message.value("status").toString(), QString("server"));

    StubTime::cleanCurrentDateTime();
}

void MulticastProtocolTests::testWeakerNode()
{
    StubTime::setCurrentDateTime(QDateTime::currentDateTime());

    StubDatabase::init();
    DatabaseSettings databaseSettings;
    databaseSettings.createClientId();

    MulticastProtocol protocol(m_proxyConnection, this);
    MulticastProtocolNode *node;
    QVariantMap message;
    QVariantMap *sendMessage;

    sendMessage = new QVariantMap();
    sendMessage->insert("id", "1");
    sendMessage->insert("score", "0");
    sendMessage->insert("port", "5000");
    sendMessage->insert("initialized", "10");
    sendMessage->insert("status", "client");
    protocol.processMessage(sendMessage);

    QVERIFY(protocol.nodeList().count() == 2);
    QVERIFY(protocol.nodeMap().count() == 2);

    // node list and map
    node = protocol.nodeList().first();
    QCOMPARE(node->id(), databaseSettings.clientId());
    QCOMPARE(node->score(), protocol.currentNode()->score());
    QCOMPARE(node->initialized(), (uint) 0);
    QCOMPARE(node->status(), MulticastProtocol::INITIALIZING);
    QCOMPARE(node->port(), (uint) 8081);
    QVERIFY(node == protocol.nodeMap().value(node->id()));

    node = protocol.nodeList().at(1);
    QCOMPARE(node->id(), (uint) 1);
    QCOMPARE(node->score(), (uint) 0);
    QCOMPARE(node->initialized(), (uint) 10);
    QCOMPARE(node->status(), MulticastProtocol::CLIENT);
    QCOMPARE(node->port(), (uint) 5000);
    QVERIFY(node == protocol.nodeMap().value(node->id()));

    // status
    node->status() == MulticastProtocol::INITIALIZING;

    // message
    message = protocol.message();
    QCOMPARE(message.value("id").toUInt(), databaseSettings.clientId());
    QCOMPARE(message.value("score").toUInt(), protocol.currentNode()->score());
    QCOMPARE(message.value("port").toInt(), 8081);
    QCOMPARE(message.value("initialized").toUInt(), (uint) 0);
    QCOMPARE(message.value("status").toString(), QString("initializing"));

    // initialized!
    protocol.initialized();

    QCOMPARE(protocol.nodeList().count(), 2);
    QCOMPARE(protocol.nodeMap().count(), 2);

    // status
    node->status() == MulticastProtocol::SERVER;

    // message
    message = protocol.message();
    QCOMPARE(message.value("id").toUInt(), databaseSettings.clientId());
    QCOMPARE(message.value("score").toUInt(), protocol.currentNode()->score());
    QCOMPARE(message.value("port").toInt(), 8081);
    QVERIFY(message.value("initialized").toUInt() > (uint) 0);
    QCOMPARE(message.value("status").toString(), QString("server"));

    // wait!
    StubTime::addSecs(5 * period);

    QCOMPARE(protocol.nodeList().count(), 1);
    QCOMPARE(protocol.nodeMap().count(), 1);

    // status
    node->status() == MulticastProtocol::SERVER;

    // message
    message = protocol.message();
    QCOMPARE(message.value("id").toUInt(), databaseSettings.clientId());
    QCOMPARE(message.value("score").toUInt(), protocol.currentNode()->score());
    QCOMPARE(message.value("port").toInt(), 8081);
    QVERIFY(message.value("initialized").toUInt() > (uint) 0);
    QCOMPARE(message.value("status").toString(), QString("server"));

    StubTime::cleanCurrentDateTime();
}

void MulticastProtocolTests::testStrongerNode()
{
    StubTime::setCurrentDateTime(QDateTime::currentDateTime());

    StubDatabase::init();
    DatabaseSettings databaseSettings;
    databaseSettings.createClientId();

    MulticastProtocol protocol(m_proxyConnection, this);
    MulticastProtocolNode *node;
    QVariantMap message;
    QVariantMap *sendMessage;

    sendMessage = new QVariantMap();
    sendMessage->insert("id", "1");
    sendMessage->insert("score", "1000000");
    sendMessage->insert("port", "5000");
    sendMessage->insert("initialized", "10");
    sendMessage->insert("status", "client");
    protocol.processMessage(sendMessage);

    QVERIFY(protocol.nodeList().count() == 2);
    QVERIFY(protocol.nodeMap().count() == 2);

    // node list and map
    node = protocol.nodeList().first();
    QCOMPARE(node->id(), (uint) 1);
    QCOMPARE(node->score(), (uint) 1000000);
    QCOMPARE(node->initialized(), (uint) 10);
    QCOMPARE(node->status(), MulticastProtocol::CLIENT);
    QCOMPARE(node->port(), (uint) 5000);
    QVERIFY(node == protocol.nodeMap().value(node->id()));

    node = protocol.nodeList().at(1);
    QCOMPARE(node->id(), databaseSettings.clientId());
    QCOMPARE(node->score(), protocol.currentNode()->score());
    QCOMPARE(node->initialized(), (uint) 0);
    QCOMPARE(node->status(), MulticastProtocol::INITIALIZING);
    QCOMPARE(node->port(), (uint) 8081);
    QVERIFY(node == protocol.nodeMap().value(node->id()));

    // status
    node->status() == MulticastProtocol::INITIALIZING;

    // message
    message = protocol.message();
    QCOMPARE(message.value("id").toUInt(), databaseSettings.clientId());
    QCOMPARE(message.value("score").toUInt(), protocol.currentNode()->score());
    QCOMPARE(message.value("port").toInt(), 8081);
    QCOMPARE(message.value("initialized").toUInt(), (uint) 0);
    QCOMPARE(message.value("status").toString(), QString("initializing"));

    // initialized!
    protocol.initialized();

    QCOMPARE(protocol.nodeList().count(), 2);
    QCOMPARE(protocol.nodeMap().count(), 2);

    // status
    node->status() == MulticastProtocol::CLIENT;

    // message
    message = protocol.message();
    QCOMPARE(message.value("id").toUInt(), databaseSettings.clientId());
    QCOMPARE(message.value("score").toUInt(), protocol.currentNode()->score());
    QCOMPARE(message.value("port").toInt(), 8081);
    QVERIFY(message.value("initialized").toUInt() > (uint) 0);
    QCOMPARE(message.value("status").toString(), QString("client"));

    // wait!
    StubTime::addSecs(5 * period);

    QCOMPARE(protocol.nodeList().count(), 1);
    QCOMPARE(protocol.nodeMap().count(), 1);

    // status
    node->status() == MulticastProtocol::SERVER;

    // message
    message = protocol.message();
    QCOMPARE(message.value("id").toUInt(), databaseSettings.clientId());
    QCOMPARE(message.value("score").toUInt(), protocol.currentNode()->score());
    QCOMPARE(message.value("port").toInt(), 8081);
    QVERIFY(message.value("initialized").toUInt() > (uint) 0);
    QCOMPARE(message.value("status").toString(), QString("server"));

    StubTime::cleanCurrentDateTime();
}

/*
void MulticastProtocolTests::testGetInstance()
{
    QVERIFY(CommunicationManager::getInstance() != NULL);
}

void MulticastProtocolTests::testGetCommunicationInstancesWhenCreated()
{
    CommunicationManager *communicationManager = new CommunicationManager;
    CommunicationInstance *instance;

    QVERIFY(communicationManager->getCommunicationInstances().count() == 1);

    instance = communicationManager->getCommunicationInstances().first();
    QCOMPARE(instance->id(), communicationManager->myId());
    QCOMPARE(instance->score(), communicationManager->myScore());
}

void MulticastProtocolTests::testGetCommunicationInstancesWhenOneHearbeatIsReceived()
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

void MulticastProtocolTests::testGetCommunicationInstancesWhenMultipleHearbeatsAreReceived()
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

void MulticastProtocolTests::testGetCommunicationInstancesWhenOneHearbeatWithChangedInfoAreReceived()
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

void MulticastProtocolTests::testGetCommunicationInstancesWhenMultipleHearbeatsWithChangedInfoAreReceived()
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

void MulticastProtocolTests::testGetCommunicationInstancesWhenNoHearbeatsAreReceived()
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
    StubTime::addSecs(2 * period + 1 * second);

    QCOMPARE(communicationManager->getCommunicationInstances().count(), 3);

    QVariantMap message3;
    message3.insert("id", "unique-client-id-1");
    message3.insert("score", "1");
    communicationManager->processMessage(&message3);

    //QTest::qSleep(1 * period);
    StubTime::addSecs(1 * period);

    QCOMPARE(communicationManager->getCommunicationInstances().count(), 1);

    //QTest::qSleep(1 * period + 1 * second);
    StubTime::addSecs(2 * period + 1 * second);

    QCOMPARE(communicationManager->getCommunicationInstances().count(), 0);

    QVariantMap message4;
    message4.insert("id", "unique-client-id-1");
    message4.insert("score", "1");
    communicationManager->processMessage(&message4);

    QCOMPARE(communicationManager->getCommunicationInstances().count(), 1);

    StubTime::cleanCurrentDateTime();
}

void MulticastProtocolTests::testGetServerWhenCreated()
{
    CommunicationManager *communicationManager = new CommunicationManager;

    QVERIFY(communicationManager->getServer() != NULL);
    QVERIFY(communicationManager->getServer()->id() == communicationManager->myId());
}

void MulticastProtocolTests::testGetServerWhenOneHeartbeatIsReceived()
{
    CommunicationManager *communicationManager = new CommunicationManager;

    QVERIFY(communicationManager->getServer() != NULL);

    QCOMPARE(communicationManager->getServer()->id(), communicationManager->myId());
}

void MulticastProtocolTests::testGetServerWhenMultipleHeartbeatsAreReceived()
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

void MulticastProtocolTests::testGetServerWhenInitializing()
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

void MulticastProtocolTests::testGetServerWhenNoHeartbeatsAreReceived()
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
    StubTime::addSecs(3 * period + 1 * second);

    QVERIFY(communicationManager->getServer() == NULL);

    QVariantMap message2;
    message2.insert("id", "unique-client-id-1");
    message2.insert("score", "1");
    communicationManager->processMessage(&message2);

    QVERIFY(communicationManager->getServer() != NULL);

    StubTime::cleanCurrentDateTime();
}

void MulticastProtocolTests::testMyStatusInitializing()
{
    CommunicationManager *communicationManager = new CommunicationManager;

    QCOMPARE(communicationManager->myStatus(), CommunicationManager::INITIALIZING);
}

void MulticastProtocolTests::testMyStatusClient()
{
    CommunicationManager *communicationManager = new CommunicationManager;

    QVariantMap message;
    message.insert("id", "unique-client-id-stronger");
    message.insert("score", "100");
    message.insert("status", "server");
    communicationManager->processMessage(&message);

    QCOMPARE(communicationManager->myStatus(), CommunicationManager::INITIALIZING);

    communicationManager->initialized();

    /*
    for (int i = 0; i < communicationManager->getCommunicationInstances().size(); ++i)
    {
        qDebug() << communicationManager->getCommunicationInstances().at(i)->id()
                 << communicationManager->getCommunicationInstances().at(i)->score()
                 << communicationManager->getCommunicationInstances().at(i)->status();
    }
    *//*

    QCOMPARE(communicationManager->myStatus(), CommunicationManager::CLIENT);
}

void MulticastProtocolTests::testMyStatusServer()
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

void MulticastProtocolTests::testMyStatusServerAfterCreated()
{
    CommunicationManager *communicationManager = new CommunicationManager;

    QCOMPARE(communicationManager->myStatus(), CommunicationManager::INITIALIZING);

    communicationManager->initialized();
    QCOMPARE(communicationManager->myStatus(), CommunicationManager::SERVER);
}

void MulticastProtocolTests::testMyStatusServerWhenNoHeartbeatsAreReceived()
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
    StubTime::addSecs(2 * period + 1 * second);

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
*/

DECLARE_TEST(MulticastProtocolTests)

#include "multicastprotocoltests.moc"
