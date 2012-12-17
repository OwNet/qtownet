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
    void testInitializingNode();
    void testMultipleNodes();
    void testMultipleWeakerNodes();

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
    QVERIFY(protocol.currentNode()->status() == MulticastProtocol::INITIALIZING);

    // message
    message = protocol.message();
    QCOMPARE(message.value("id").toUInt(), databaseSettings.clientId());
    QCOMPARE(message.value("score").toUInt(), protocol.currentNode()->score());
    QCOMPARE(message.value("port").toInt(), 8081);
    QCOMPARE(message.value("initialized").toUInt(), (uint) 0);
    QCOMPARE(message.value("status").toString(), QString("initializing"));

    // server
    QVERIFY(protocol.serverNode() == NULL);

    // wait!
    StubTime::addSecs(5 * period);

    QCOMPARE(protocol.nodeList().count(), 1);
    QCOMPARE(protocol.nodeMap().count(), 1);

    // status
    QVERIFY(protocol.currentNode()->status() == MulticastProtocol::INITIALIZING);

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
    QVERIFY(protocol.currentNode()->status() == MulticastProtocol::SERVER);

    // message
    message = protocol.message();
    QCOMPARE(message.value("id").toUInt(), databaseSettings.clientId());
    QCOMPARE(message.value("score").toUInt(), protocol.currentNode()->score());
    QCOMPARE(message.value("port").toInt(), 8081);
    QVERIFY(message.value("initialized").toUInt() > (uint) 0);
    QCOMPARE(message.value("status").toString(), QString("server"));

    // server
    QVERIFY(protocol.serverNode() == NULL);

    // wait!
    StubTime::addSecs(5 * period);

    QCOMPARE(protocol.nodeList().count(), 1);
    QCOMPARE(protocol.nodeMap().count(), 1);

    // status
    QVERIFY(protocol.currentNode()->status() == MulticastProtocol::SERVER);

    // message
    message = protocol.message();
    QCOMPARE(message.value("id").toUInt(), databaseSettings.clientId());
    QCOMPARE(message.value("score").toUInt(), protocol.currentNode()->score());
    QCOMPARE(message.value("port").toInt(), 8081);
    QVERIFY(message.value("initialized").toUInt() > (uint) 0);
    QCOMPARE(message.value("status").toString(), QString("server"));

    // server
    QVERIFY(protocol.serverNode() == NULL);

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
    QVERIFY(protocol.currentNode()->status() == MulticastProtocol::INITIALIZING);

    // message
    message = protocol.message();
    QCOMPARE(message.value("id").toUInt(), databaseSettings.clientId());
    QCOMPARE(message.value("score").toUInt(), protocol.currentNode()->score());
    QCOMPARE(message.value("port").toInt(), 8081);
    QCOMPARE(message.value("initialized").toUInt(), (uint) 0);
    QCOMPARE(message.value("status").toString(), QString("initializing"));

    // server
    QVERIFY(protocol.serverNode() == NULL);

    // initialized!
    protocol.initialized();

    QCOMPARE(protocol.nodeList().count(), 2);
    QCOMPARE(protocol.nodeMap().count(), 2);

    // status
    QVERIFY(protocol.currentNode()->status() == MulticastProtocol::SERVER);

    // message
    message = protocol.message();
    QCOMPARE(message.value("id").toUInt(), databaseSettings.clientId());
    QCOMPARE(message.value("score").toUInt(), protocol.currentNode()->score());
    QCOMPARE(message.value("port").toInt(), 8081);
    QVERIFY(message.value("initialized").toUInt() > (uint) 0);
    QCOMPARE(message.value("status").toString(), QString("server"));

    // server
    QVERIFY(protocol.serverNode() == NULL);

    // wait!
    StubTime::addSecs(5 * period);

    QCOMPARE(protocol.nodeList().count(), 1);
    QCOMPARE(protocol.nodeMap().count(), 1);

    // status
    QVERIFY(protocol.currentNode()->status() == MulticastProtocol::SERVER);

    // message
    message = protocol.message();
    QCOMPARE(message.value("id").toUInt(), databaseSettings.clientId());
    QCOMPARE(message.value("score").toUInt(), protocol.currentNode()->score());
    QCOMPARE(message.value("port").toInt(), 8081);
    QVERIFY(message.value("initialized").toUInt() > (uint) 0);
    QCOMPARE(message.value("status").toString(), QString("server"));

    // server
    QVERIFY(protocol.serverNode() == NULL);

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
    sendMessage->insert("status", "server");
    protocol.processMessage(sendMessage);

    QVERIFY(protocol.nodeList().count() == 2);
    QVERIFY(protocol.nodeMap().count() == 2);

    // node list and map
    node = protocol.nodeList().first();
    QCOMPARE(node->id(), (uint) 1);
    QCOMPARE(node->score(), (uint) 1000000);
    QCOMPARE(node->initialized(), (uint) 10);
    QCOMPARE(node->status(), MulticastProtocol::SERVER);
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
    QVERIFY(protocol.currentNode()->status() == MulticastProtocol::INITIALIZING);

    // message
    message = protocol.message();
    QCOMPARE(message.value("id").toUInt(), databaseSettings.clientId());
    QCOMPARE(message.value("score").toUInt(), protocol.currentNode()->score());
    QCOMPARE(message.value("port").toInt(), 8081);
    QCOMPARE(message.value("initialized").toUInt(), (uint) 0);
    QCOMPARE(message.value("status").toString(), QString("initializing"));

    // server
    QVERIFY(protocol.serverNode() != NULL);

    // initialized!
    protocol.initialized();

    QCOMPARE(protocol.nodeList().count(), 2);
    QCOMPARE(protocol.nodeMap().count(), 2);

    // status
    QVERIFY(protocol.currentNode()->status() == MulticastProtocol::CLIENT);

    // message
    message = protocol.message();
    QCOMPARE(message.value("id").toUInt(), databaseSettings.clientId());
    QCOMPARE(message.value("score").toUInt(), protocol.currentNode()->score());
    QCOMPARE(message.value("port").toInt(), 8081);
    QVERIFY(message.value("initialized").toUInt() > (uint) 0);
    QCOMPARE(message.value("status").toString(), QString("client"));

    // server
    QVERIFY(protocol.serverNode() == protocol.nodeList().first());

    // wait!
    StubTime::addSecs(5 * period);

    QCOMPARE(protocol.nodeList().count(), 1);
    QCOMPARE(protocol.nodeMap().count(), 1);

    // status
    QVERIFY(protocol.currentNode()->status() == MulticastProtocol::SERVER);

    // message
    message = protocol.message();
    QCOMPARE(message.value("id").toUInt(), databaseSettings.clientId());
    QCOMPARE(message.value("score").toUInt(), protocol.currentNode()->score());
    QCOMPARE(message.value("port").toInt(), 8081);
    QVERIFY(message.value("initialized").toUInt() > (uint) 0);
    QCOMPARE(message.value("status").toString(), QString("server"));

    // server
    QVERIFY(protocol.serverNode() == NULL);

    StubTime::cleanCurrentDateTime();
}

void MulticastProtocolTests::testInitializingNode()
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
    sendMessage->insert("initialized", "0");
    sendMessage->insert("status", "initializing");
    protocol.processMessage(sendMessage);

    QVERIFY(protocol.nodeList().count() == 2);
    QVERIFY(protocol.nodeMap().count() == 2);

    // node list and map
    node = protocol.nodeList().first();
    QCOMPARE(node->id(), (uint) 1);
    QCOMPARE(node->score(), (uint) 1000000);
    QCOMPARE(node->initialized(), (uint) 0);
    QCOMPARE(node->status(), MulticastProtocol::INITIALIZING);
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
    QVERIFY(protocol.currentNode()->status() == MulticastProtocol::INITIALIZING);

    // message
    message = protocol.message();
    QCOMPARE(message.value("id").toUInt(), databaseSettings.clientId());
    QCOMPARE(message.value("score").toUInt(), protocol.currentNode()->score());
    QCOMPARE(message.value("port").toInt(), 8081);
    QCOMPARE(message.value("initialized").toUInt(), (uint) 0);
    QCOMPARE(message.value("status").toString(), QString("initializing"));

    // server
    QVERIFY(protocol.serverNode() == NULL);

    // initialized!
    protocol.initialized();

    QCOMPARE(protocol.nodeList().count(), 2);
    QCOMPARE(protocol.nodeMap().count(), 2);

    // status
    QVERIFY(protocol.currentNode()->status() == MulticastProtocol::SERVER);

    // message
    message = protocol.message();
    QCOMPARE(message.value("id").toUInt(), databaseSettings.clientId());
    QCOMPARE(message.value("score").toUInt(), protocol.currentNode()->score());
    QCOMPARE(message.value("port").toInt(), 8081);
    QVERIFY(message.value("initialized").toUInt() > (uint) 0);
    QCOMPARE(message.value("status").toString(), QString("server"));

    // server
    QVERIFY(protocol.serverNode() == NULL);

    // stronger node initialized!

    sendMessage = new QVariantMap();
    sendMessage->insert("id", "1");
    sendMessage->insert("score", "1000000");
    sendMessage->insert("port", "5000");
    sendMessage->insert("initialized", "10");
    sendMessage->insert("status", "server");
    protocol.processMessage(sendMessage);

    QCOMPARE(protocol.nodeList().count(), 2);
    QCOMPARE(protocol.nodeMap().count(), 2);

    // status
    QVERIFY(protocol.currentNode()->status() == MulticastProtocol::CLIENT);

    // message
    message = protocol.message();
    QCOMPARE(message.value("id").toUInt(), databaseSettings.clientId());
    QCOMPARE(message.value("score").toUInt(), protocol.currentNode()->score());
    QCOMPARE(message.value("port").toInt(), 8081);
    QVERIFY(message.value("initialized").toUInt() > (uint) 0);
    QCOMPARE(message.value("status").toString(), QString("client"));

    // server
    QVERIFY(protocol.serverNode() == protocol.nodeList().first());

    StubTime::cleanCurrentDateTime();
}

void MulticastProtocolTests::testMultipleNodes()
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
    sendMessage->insert("score", "2000000");
    sendMessage->insert("port", "5000");
    sendMessage->insert("initialized", "10");
    sendMessage->insert("status", "server");
    protocol.processMessage(sendMessage);

    sendMessage = new QVariantMap();
    sendMessage->insert("id", "2");
    sendMessage->insert("score", "1000000");
    sendMessage->insert("port", "5000");
    sendMessage->insert("initialized", "9");
    sendMessage->insert("status", "client");
    protocol.processMessage(sendMessage);

    QVERIFY(protocol.nodeList().count() == 3);
    QVERIFY(protocol.nodeMap().count() == 3);

    // node list and map
    node = protocol.nodeList().first();
    QCOMPARE(node->id(), (uint) 1);
    QCOMPARE(node->score(), (uint) 2000000);
    QCOMPARE(node->initialized(), (uint) 10);
    QCOMPARE(node->status(), MulticastProtocol::SERVER);
    QCOMPARE(node->port(), (uint) 5000);
    QVERIFY(node == protocol.nodeMap().value(node->id()));

    node = protocol.nodeList().at(1);
    QCOMPARE(node->id(), (uint) 2);
    QCOMPARE(node->score(), (uint) 1000000);
    QCOMPARE(node->initialized(), (uint) 9);
    QCOMPARE(node->status(), MulticastProtocol::CLIENT);
    QCOMPARE(node->port(), (uint) 5000);
    QVERIFY(node == protocol.nodeMap().value(node->id()));

    node = protocol.nodeList().at(2);
    QCOMPARE(node->id(), databaseSettings.clientId());
    QCOMPARE(node->score(), protocol.currentNode()->score());
    QCOMPARE(node->initialized(), (uint) 0);
    QCOMPARE(node->status(), MulticastProtocol::INITIALIZING);
    QCOMPARE(node->port(), (uint) 8081);
    QVERIFY(node == protocol.nodeMap().value(node->id()));

    // status
    QVERIFY(protocol.currentNode()->status() == MulticastProtocol::INITIALIZING);

    // message
    message = protocol.message();
    QCOMPARE(message.value("id").toUInt(), databaseSettings.clientId());
    QCOMPARE(message.value("score").toUInt(), protocol.currentNode()->score());
    QCOMPARE(message.value("port").toInt(), 8081);
    QCOMPARE(message.value("initialized").toUInt(), (uint) 0);
    QCOMPARE(message.value("status").toString(), QString("initializing"));

    // server
    QVERIFY(protocol.serverNode() != NULL);

    // initialized!
    protocol.initialized();

    QCOMPARE(protocol.nodeList().count(), 3);
    QCOMPARE(protocol.nodeMap().count(), 3);

    // status
    QVERIFY(protocol.currentNode()->status() == MulticastProtocol::CLIENT);

    // message
    message = protocol.message();
    QCOMPARE(message.value("id").toUInt(), databaseSettings.clientId());
    QCOMPARE(message.value("score").toUInt(), protocol.currentNode()->score());
    QCOMPARE(message.value("port").toInt(), 8081);
    QVERIFY(message.value("initialized").toUInt() > (uint) 0);
    QCOMPARE(message.value("status").toString(), QString("client"));

    // server
    QVERIFY(protocol.serverNode() != NULL);

    // wait!
    StubTime::addSecs(2 * period + 1);

    QCOMPARE(protocol.nodeList().count(), 3);
    QCOMPARE(protocol.nodeMap().count(), 3);

    // status
    QVERIFY(protocol.currentNode()->status() == MulticastProtocol::CLIENT);

    // message
    message = protocol.message();
    QCOMPARE(message.value("id").toUInt(), databaseSettings.clientId());
    QCOMPARE(message.value("score").toUInt(), protocol.currentNode()->score());
    QCOMPARE(message.value("port").toInt(), 8081);
    QVERIFY(message.value("initialized").toUInt() > (uint) 0);
    QCOMPARE(message.value("status").toString(), QString("client"));

    // server
    QVERIFY(protocol.serverNode() != NULL);

    sendMessage = new QVariantMap();
    sendMessage->insert("id", "2");
    sendMessage->insert("score", "3000000"); // changed
    sendMessage->insert("port", "5000");
    sendMessage->insert("initialized", "9");
    sendMessage->insert("status", "server"); // changed
    protocol.processMessage(sendMessage);

    // wait!
    StubTime::addSecs(1 * period);

    QVERIFY(protocol.nodeList().count() == 2);
    QVERIFY(protocol.nodeMap().count() == 2);

    // node list and map
    node = protocol.nodeList().first();
    QCOMPARE(node->id(), (uint) 2);
    QCOMPARE(node->score(), (uint) 3000000);
    QCOMPARE(node->initialized(), (uint) 9);
    QCOMPARE(node->status(), MulticastProtocol::SERVER);
    QCOMPARE(node->port(), (uint) 5000);
    QVERIFY(node == protocol.nodeMap().value(node->id()));

    node = protocol.nodeList().at(1);
    QCOMPARE(node->id(), databaseSettings.clientId());
    QCOMPARE(node->score(), protocol.currentNode()->score());
    QVERIFY(node->initialized() > (uint) 0);
    QCOMPARE(node->status(), MulticastProtocol::CLIENT);
    QCOMPARE(node->port(), (uint) 8081);
    QVERIFY(node == protocol.nodeMap().value(node->id()));

    // status
    QVERIFY(protocol.currentNode()->status() == MulticastProtocol::CLIENT);

    // message
    message = protocol.message();
    QCOMPARE(message.value("id").toUInt(), databaseSettings.clientId());
    QCOMPARE(message.value("score").toUInt(), protocol.currentNode()->score());
    QCOMPARE(message.value("port").toInt(), 8081);
    QVERIFY(message.value("initialized").toUInt() > (uint) 0);
    QCOMPARE(message.value("status").toString(), QString("client"));

    StubTime::cleanCurrentDateTime();
}

void MulticastProtocolTests::testMultipleWeakerNodes()
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
    sendMessage->insert("status", "client");
    protocol.processMessage(sendMessage);

    sendMessage = new QVariantMap();
    sendMessage->insert("id", "2");
    sendMessage->insert("score", "0");
    sendMessage->insert("status", "client");
    protocol.processMessage(sendMessage);

    sendMessage = new QVariantMap();
    sendMessage->insert("id", "3");
    sendMessage->insert("score", "0");
    sendMessage->insert("status", "server");
    protocol.processMessage(sendMessage);

    QVERIFY(protocol.nodeList().count() == 4);
    QVERIFY(protocol.nodeMap().count() == 4);

    // node list and map
    node = protocol.nodeList().first();
    QCOMPARE(node->id(), databaseSettings.clientId());
    QCOMPARE(node->score(), protocol.currentNode()->score());
    QVERIFY(node == protocol.nodeMap().value(node->id()));

    node = protocol.nodeList().at(1);
    QCOMPARE(node->id(), (uint) 1);
    QCOMPARE(node->score(), (uint) 0);
    QVERIFY(node == protocol.nodeMap().value(node->id()));

    node = protocol.nodeList().at(2);
    QCOMPARE(node->id(), (uint) 2);
    QCOMPARE(node->score(), (uint) 0);
    QVERIFY(node == protocol.nodeMap().value(node->id()));

    node = protocol.nodeList().at(3);
    QCOMPARE(node->id(), (uint) 3);
    QCOMPARE(node->score(), (uint) 0);
    QVERIFY(node == protocol.nodeMap().value(node->id()));

    // status
    QVERIFY(protocol.currentNode()->status() == MulticastProtocol::INITIALIZING);

    // message
    message = protocol.message();
    QCOMPARE(message.value("id").toUInt(), databaseSettings.clientId());
    QCOMPARE(message.value("score").toUInt(), protocol.currentNode()->score());
    QCOMPARE(message.value("port").toInt(), 8081);
    QCOMPARE(message.value("initialized").toUInt(), (uint) 0);
    QCOMPARE(message.value("status").toString(), QString("initializing"));

    // server
    QVERIFY(protocol.serverNode() == NULL);

    // initialized!
    protocol.initialized();

    QCOMPARE(protocol.nodeList().count(), 4);
    QCOMPARE(protocol.nodeMap().count(), 4);

    // status
    QVERIFY(protocol.currentNode()->status() == MulticastProtocol::SERVER);

    // message
    message = protocol.message();
    QCOMPARE(message.value("id").toUInt(), databaseSettings.clientId());
    QCOMPARE(message.value("score").toUInt(), protocol.currentNode()->score());
    QCOMPARE(message.value("port").toInt(), 8081);
    QVERIFY(message.value("initialized").toUInt() > (uint) 0);
    QCOMPARE(message.value("status").toString(), QString("server"));

    // server
    QVERIFY(protocol.serverNode() == NULL);

    StubTime::cleanCurrentDateTime();
}

DECLARE_TEST(MulticastProtocolTests)

#include "multicastprotocoltests.moc"
