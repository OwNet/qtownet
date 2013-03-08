#include <QDebug>

#include "autotest.h"
#include "stub/stubtime.h"
#include "stub/stubdatabase.h"
#include "proxyconnection.h"
#include "session.h"
#include "settings.h"
#include "databasesettings.h"

#include "multicastprotocol.h"
#include "multicastprotocolnode.h"

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
    StubDatabase::init();
    m_proxyConnection = new ProxyConnection();
}

void MulticastProtocolTests::cleanupTestCase()
{
    StubDatabase::close();
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

    QCOMPARE(protocol.nodes().count(), 1);

    // node list
    node = protocol.nodes().first();
    QCOMPARE(node->id(), databaseSettings.clientId());
    QCOMPARE(node->score(), protocol.currentNode()->score());
    QCOMPARE(node->initialized(), (uint) 0);
    QCOMPARE(node->status(), MulticastProtocol::INITIALIZING);
    QCOMPARE(node->port(), (uint) 8081);

    // status
    QVERIFY(protocol.currentNode()->status() == MulticastProtocol::INITIALIZING);

    // message
    message = protocol.message();
    QCOMPARE(message.value("id").toString(), databaseSettings.clientId());
    QCOMPARE(message.value("score").toUInt(), protocol.currentNode()->score());
    QCOMPARE(message.value("port").toInt(), 8081);
    QCOMPARE(message.value("initialized").toUInt(), (uint) 0);
    QCOMPARE(message.value("status").toString(), QString("initializing"));

    // server
    QVERIFY(protocol.serverNode() == NULL);

    // wait!
    StubTime::addSecs(5 * period);
    protocol.update();

    QCOMPARE(protocol.nodes().count(), 1);

    // status
    QVERIFY(protocol.currentNode()->status() == MulticastProtocol::INITIALIZING);

    // message
    message = protocol.message();
    QCOMPARE(message.value("id").toString(), databaseSettings.clientId());
    QCOMPARE(message.value("score").toUInt(), protocol.currentNode()->score());
    QCOMPARE(message.value("port").toInt(), 8081);
    QCOMPARE(message.value("initialized").toUInt(), (uint) 0);
    QCOMPARE(message.value("status").toString(), QString("initializing"));

    // initialized!
    protocol.initialized();
    protocol.update();

    QCOMPARE(protocol.nodes().count(), 1);

    // status
    QVERIFY(protocol.currentNode()->status() == MulticastProtocol::SERVER);

    // message
    message = protocol.message();
    QCOMPARE(message.value("id").toString(), databaseSettings.clientId());
    QCOMPARE(message.value("score").toUInt(), protocol.currentNode()->score());
    QCOMPARE(message.value("port").toInt(), 8081);
    QVERIFY(message.value("initialized").toUInt() > (uint) 0);
    QCOMPARE(message.value("status").toString(), QString("server"));

    // server
    QVERIFY(protocol.serverNode() == NULL);

    // wait!
    StubTime::addSecs(5 * period);
    protocol.update();

    QCOMPARE(protocol.nodes().count(), 1);

    // status
    QVERIFY(protocol.currentNode()->status() == MulticastProtocol::SERVER);

    // message
    message = protocol.message();
    QCOMPARE(message.value("id").toString(), databaseSettings.clientId());
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
    Settings settings;

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
    sendMessage->insert("address", "10.10.10.10"); // test IPs
    sendMessage->insert("workspace_id", settings.value("current_workspace/id").toString());
    sendMessage->insert("workspace_name", settings.value("current_workspace/name").toString());

    protocol.processMessage(*sendMessage);
    protocol.update();

    QCOMPARE(protocol.nodes().count(), 2);

    // node list and map
    node = protocol.nodes().first();
    QCOMPARE(node->id(), databaseSettings.clientId());
    QCOMPARE(node->score(), protocol.currentNode()->score());
    QCOMPARE(node->initialized(), (uint) 0);
    QCOMPARE(node->status(), MulticastProtocol::INITIALIZING);
    QCOMPARE(node->port(), (uint) 8081);
    QCOMPARE(node->address(), QString("127.0.0.1"));

    node = protocol.nodes().at(1);
    QCOMPARE(node->id(), QString("1"));
    QCOMPARE(node->score(), (uint) 0);
    QCOMPARE(node->initialized(), (uint) 10);
    QCOMPARE(node->status(), MulticastProtocol::CLIENT);
    QCOMPARE(node->port(), (uint) 5000);
    QCOMPARE(node->address(), QString("10.10.10.10"));

    // session
    QObject parent;
    ISession *session = m_proxyConnection->session(&parent);
    QCOMPARE(session->serverId(),
             m_proxyConnection->databaseSettings()->clientId());
    QCOMPARE(session->value("multicast_server_port").toUInt(),
             (uint) 0);

    QCOMPARE(session->availableClients().value("1").toString(),
             QString("10.10.10.10:5000"));

    // status
    QVERIFY(protocol.currentNode()->status() == MulticastProtocol::INITIALIZING);

    // message
    message = protocol.message();
    QCOMPARE(message.value("id").toString(), databaseSettings.clientId());
    QCOMPARE(message.value("score").toUInt(), protocol.currentNode()->score());
    QCOMPARE(message.value("port").toInt(), 8081);
    QCOMPARE(message.value("initialized").toUInt(), (uint) 0);
    QCOMPARE(message.value("status").toString(), QString("initializing"));

    // server
    QVERIFY(protocol.serverNode() == NULL);

    // initialized!
    protocol.initialized();
    protocol.update();

    QCOMPARE(protocol.nodes().count(), 2);

    // message
    message = protocol.message();
    QCOMPARE(message.value("id").toString(), databaseSettings.clientId());
    QCOMPARE(message.value("score").toUInt(), protocol.currentNode()->score());
    QCOMPARE(message.value("port").toInt(), 8081);
    QVERIFY(message.value("initialized").toUInt() > (uint) 0);
    QCOMPARE(message.value("status").toString(), QString("server"));

    // status
    QVERIFY(protocol.currentNode()->status() == MulticastProtocol::SERVER);

    // server
    QVERIFY(protocol.serverNode() == NULL);

    // wait!
    StubTime::addSecs(5 * period);
    protocol.update();

    QCOMPARE(protocol.nodes().count(), 1);

    // status
    QVERIFY(protocol.currentNode()->status() == MulticastProtocol::SERVER);

    // message
    message = protocol.message();
    QCOMPARE(message.value("id").toString(), databaseSettings.clientId());
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

    DatabaseSettings databaseSettings;
    databaseSettings.createClientId();
    Settings settings;

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
    sendMessage->insert("address", "10.10.10.10");
    sendMessage->insert("workspace_id", settings.value("current_workspace/id").toString());
    sendMessage->insert("workspace_name", settings.value("current_workspace/name").toString());

    protocol.processMessage(*sendMessage);
    protocol.update();

    QVERIFY(protocol.nodes().count() == 2);

    // node list and map
    node = protocol.nodes().first();
    QCOMPARE(node->id(), QString("1"));
    QCOMPARE(node->score(), (uint) 1000000);
    QCOMPARE(node->initialized(), (uint) 10);
    QCOMPARE(node->status(), MulticastProtocol::SERVER);
    QCOMPARE(node->port(), (uint) 5000);
    QCOMPARE(node->address(), QString("10.10.10.10"));

    node = protocol.nodes().at(1);
    QCOMPARE(node->id(), databaseSettings.clientId());
    QCOMPARE(node->score(), protocol.currentNode()->score());
    QCOMPARE(node->initialized(), (uint) 0);
    QCOMPARE(node->status(), MulticastProtocol::INITIALIZING);
    QCOMPARE(node->port(), (uint) 8081);

    // status
    QVERIFY(protocol.currentNode()->status() == MulticastProtocol::INITIALIZING);

    // message
    message = protocol.message();
    QCOMPARE(message.value("id").toString(), databaseSettings.clientId());
    QCOMPARE(message.value("score").toUInt(), protocol.currentNode()->score());
    QCOMPARE(message.value("port").toInt(), 8081);
    QCOMPARE(message.value("initialized").toUInt(), (uint) 0);
    QCOMPARE(message.value("status").toString(), QString("initializing"));

    // server
    QVERIFY(protocol.serverNode() != NULL);

    // session
    QObject parent;
    ISession *session = m_proxyConnection->session(&parent);
    QCOMPARE(session->availableClients().value(session->serverId()).toString(),
             QString("10.10.10.10:5000"));

    QCOMPARE(session->availableClients().value("1").toString(),
             QString("10.10.10.10:5000"));

    // initialized!

    protocol.initialized();
    protocol.update();;

    QCOMPARE(protocol.nodes().count(), 2);

    // status
    QVERIFY(protocol.currentNode()->status() == MulticastProtocol::CLIENT);

    // message
    message = protocol.message();
    QCOMPARE(message.value("id").toString(), databaseSettings.clientId());
    QCOMPARE(message.value("score").toUInt(), protocol.currentNode()->score());
    QCOMPARE(message.value("port").toInt(), 8081);
    QVERIFY(message.value("initialized").toUInt() > (uint) 0);
    QCOMPARE(message.value("status").toString(), QString("client"));

    // server
    QVERIFY(protocol.serverNode() == protocol.nodes().first());

    // wait!
    StubTime::addSecs(5 * period);
    protocol.update();

    QCOMPARE(protocol.nodes().count(), 1);

    // status
    QVERIFY(protocol.currentNode()->status() == MulticastProtocol::SERVER);

    // message
    message = protocol.message();
    QCOMPARE(message.value("id").toString(), databaseSettings.clientId());
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
    Settings settings;

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
    sendMessage->insert("workspace_id", settings.value("current_workspace/id").toString());
    sendMessage->insert("workspace_name", settings.value("current_workspace/name").toString());

    protocol.processMessage(*sendMessage);
    protocol.update();

    QVERIFY(protocol.nodes().count() == 2);

    // node list and map
    node = protocol.nodes().first();
    QCOMPARE(node->id(), QString(1));
    QCOMPARE(node->score(), (uint) 1000000);
    QCOMPARE(node->initialized(), (uint) 0);
    QCOMPARE(node->status(), MulticastProtocol::INITIALIZING);
    QCOMPARE(node->port(), (uint) 5000);

    node = protocol.nodes().at(1);
    QCOMPARE(node->id(), databaseSettings.clientId());
    QCOMPARE(node->score(), protocol.currentNode()->score());
    QCOMPARE(node->initialized(), (uint) 0);
    QCOMPARE(node->status(), MulticastProtocol::INITIALIZING);
    QCOMPARE(node->port(), (uint) 8081);

    // status
    QVERIFY(protocol.currentNode()->status() == MulticastProtocol::INITIALIZING);

    // message
    message = protocol.message();
    QCOMPARE(message.value("id").toString(), databaseSettings.clientId());
    QCOMPARE(message.value("score").toUInt(), protocol.currentNode()->score());
    QCOMPARE(message.value("port").toInt(), 8081);
    QCOMPARE(message.value("initialized").toUInt(), (uint) 0);
    QCOMPARE(message.value("status").toString(), QString("initializing"));

    // server
    QVERIFY(protocol.serverNode() == NULL);

    // initialized!

    protocol.initialized();
    protocol.update();;

    QCOMPARE(protocol.nodes().count(), 2);

    // status
    QVERIFY(protocol.currentNode()->status() == MulticastProtocol::SERVER);

    // message
    message = protocol.message();
    QCOMPARE(message.value("id").toString(), databaseSettings.clientId());
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

    protocol.processMessage(*sendMessage);
    protocol.update();

    QCOMPARE(protocol.nodes().count(), 2);

    // status
    QVERIFY(protocol.currentNode()->status() == MulticastProtocol::CLIENT);

    // message
    message = protocol.message();
    QCOMPARE(message.value("id").toString(), databaseSettings.clientId());
    QCOMPARE(message.value("score").toUInt(), protocol.currentNode()->score());
    QCOMPARE(message.value("port").toInt(), 8081);
    QVERIFY(message.value("initialized").toUInt() > (uint) 0);
    QCOMPARE(message.value("status").toString(), QString("client"));

    // server
    QVERIFY(protocol.serverNode() == protocol.nodes().first());

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
    Settings settings;

    sendMessage = new QVariantMap();
    sendMessage->insert("id", "1");
    sendMessage->insert("score", "2000000");
    sendMessage->insert("port", "5000");
    sendMessage->insert("initialized", "10");
    sendMessage->insert("status", "server");
    sendMessage->insert("workspace_id", settings.value("current_workspace/id").toString());
    sendMessage->insert("workspace_name", settings.value("current_workspace/name").toString());

    protocol.processMessage(*sendMessage);
    protocol.update();

    sendMessage = new QVariantMap();
    sendMessage->insert("id", "2");
    sendMessage->insert("score", "1000000");
    sendMessage->insert("port", "5000");
    sendMessage->insert("initialized", "9");
    sendMessage->insert("status", "client");
    sendMessage->insert("workspace_id", settings.value("current_workspace/id").toString());
    sendMessage->insert("workspace_name", settings.value("current_workspace/name").toString());

    protocol.processMessage(*sendMessage);
    protocol.update();

    QVERIFY(protocol.nodes().count() == 3);

    // node list and map
    node = protocol.nodes().first();
    QCOMPARE(node->id(), QString(1));
    QCOMPARE(node->score(), (uint) 2000000);
    QCOMPARE(node->initialized(), (uint) 10);
    QCOMPARE(node->status(), MulticastProtocol::SERVER);
    QCOMPARE(node->port(), (uint) 5000);

    node = protocol.nodes().at(1);
    QCOMPARE(node->id(), QString(2));
    QCOMPARE(node->score(), (uint) 1000000);
    QCOMPARE(node->initialized(), (uint) 9);
    QCOMPARE(node->status(), MulticastProtocol::CLIENT);
    QCOMPARE(node->port(), (uint) 5000);

    node = protocol.nodes().at(2);
    QCOMPARE(node->id(), databaseSettings.clientId());
    QCOMPARE(node->score(), protocol.currentNode()->score());
    QCOMPARE(node->initialized(), (uint) 0);
    QCOMPARE(node->status(), MulticastProtocol::INITIALIZING);
    QCOMPARE(node->port(), (uint) 8081);

    // status
    QVERIFY(protocol.currentNode()->status() == MulticastProtocol::INITIALIZING);

    // message
    message = protocol.message();
    QCOMPARE(message.value("id").toString(), databaseSettings.clientId());
    QCOMPARE(message.value("score").toUInt(), protocol.currentNode()->score());
    QCOMPARE(message.value("port").toInt(), 8081);
    QCOMPARE(message.value("initialized").toUInt(), (uint) 0);
    QCOMPARE(message.value("status").toString(), QString("initializing"));

    // server
    QVERIFY(protocol.serverNode() != NULL);

    // initialized!

    protocol.initialized();
    protocol.update();;

    QCOMPARE(protocol.nodes().count(), 3);

    // status
    QVERIFY(protocol.currentNode()->status() == MulticastProtocol::CLIENT);

    // message
    message = protocol.message();
    QCOMPARE(message.value("id").toString(), databaseSettings.clientId());
    QCOMPARE(message.value("score").toUInt(), protocol.currentNode()->score());
    QCOMPARE(message.value("port").toInt(), 8081);
    QVERIFY(message.value("initialized").toUInt() > (uint) 0);
    QCOMPARE(message.value("status").toString(), QString("client"));

    // server
    QVERIFY(protocol.serverNode() != NULL);

    // wait!
    StubTime::addSecs(2 * period + 1);
    protocol.update();

    QCOMPARE(protocol.nodes().count(), 3);

    // status
    QVERIFY(protocol.currentNode()->status() == MulticastProtocol::CLIENT);

    // message
    message = protocol.message();
    QCOMPARE(message.value("id").toString(), databaseSettings.clientId());
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

    protocol.processMessage(*sendMessage);
    protocol.update();

    // wait!
    StubTime::addSecs(1 * period);
    protocol.update();

    QVERIFY(protocol.nodes().count() == 2);

    // node list and map
    node = protocol.nodes().first();
    QCOMPARE(node->id(), QString("2"));
    QCOMPARE(node->score(), (uint) 3000000);
    QCOMPARE(node->initialized(), (uint) 9);
    QCOMPARE(node->status(), MulticastProtocol::SERVER);
    QCOMPARE(node->port(), (uint) 5000);

    node = protocol.nodes().at(1);
    QCOMPARE(node->id(), databaseSettings.clientId());
    QCOMPARE(node->score(), protocol.currentNode()->score());
    QVERIFY(node->initialized() > (uint) 0);
    QCOMPARE(node->status(), MulticastProtocol::CLIENT);
    QCOMPARE(node->port(), (uint) 8081);

    // status
    QVERIFY(protocol.currentNode()->status() == MulticastProtocol::CLIENT);

    // message
    message = protocol.message();
    QCOMPARE(message.value("id").toString(), databaseSettings.clientId());
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
    Settings settings;

    MulticastProtocol protocol(m_proxyConnection, this);
    MulticastProtocolNode *node;
    QVariantMap message;
    QVariantMap *sendMessage;

    sendMessage = new QVariantMap();
    sendMessage->insert("id", "1");
    sendMessage->insert("score", "0");
    sendMessage->insert("status", "client");
    sendMessage->insert("initialized", "3");    // test sort by initialized
    sendMessage->insert("workspace_id", settings.value("current_workspace/id").toString());
    sendMessage->insert("workspace_name", settings.value("current_workspace/name").toString());

    protocol.processMessage(*sendMessage);
    protocol.update();

    sendMessage = new QVariantMap();
    sendMessage->insert("id", "2");
    sendMessage->insert("score", "0");
    sendMessage->insert("status", "client");
    sendMessage->insert("initialized", "2");    // test sort by initialized
    sendMessage->insert("workspace_id", settings.value("current_workspace/id").toString());
    sendMessage->insert("workspace_name", settings.value("current_workspace/name").toString());

    protocol.processMessage(*sendMessage);
    protocol.update();

    sendMessage = new QVariantMap();
    sendMessage->insert("id", "3");
    sendMessage->insert("score", "0");
    sendMessage->insert("status", "server");
    sendMessage->insert("initialized", "1");    // test sort by initialized
    sendMessage->insert("workspace_id", settings.value("current_workspace/id").toString());
    sendMessage->insert("workspace_name", settings.value("current_workspace/name").toString());

    protocol.processMessage(*sendMessage);
    protocol.update();

    QVERIFY(protocol.nodes().count() == 4);

    // node list and map
    node = protocol.nodes().first();
    QCOMPARE(node->id(), databaseSettings.clientId());
    QCOMPARE(node->score(), protocol.currentNode()->score());

    node = protocol.nodes().at(1);
    QCOMPARE(node->id(), QString("3"));
    QCOMPARE(node->score(), (uint) 0);
    QCOMPARE(node->initialized(), (uint) 1);

    node = protocol.nodes().at(2);
    QCOMPARE(node->id(), QString("2"));
    QCOMPARE(node->score(), (uint) 0);
    QCOMPARE(node->initialized(), (uint) 2);

    node = protocol.nodes().at(3);
    QCOMPARE(node->id(), QString("1"));
    QCOMPARE(node->score(), (uint) 0);
    QCOMPARE(node->initialized(), (uint) 3);

    // status
    QVERIFY(protocol.currentNode()->status() == MulticastProtocol::INITIALIZING);

    // message
    message = protocol.message();
    QCOMPARE(message.value("id").toString(), databaseSettings.clientId());
    QCOMPARE(message.value("score").toUInt(), protocol.currentNode()->score());
    QCOMPARE(message.value("port").toInt(), 8081);
    QCOMPARE(message.value("initialized").toUInt(), (uint) 0);
    QCOMPARE(message.value("status").toString(), QString("initializing"));

    // server
    QVERIFY(protocol.serverNode() == NULL);

    // initialized!

    protocol.initialized();
    protocol.update();;

    QCOMPARE(protocol.nodes().count(), 4);

    // status 1
    QVERIFY(protocol.currentNode()->status() == MulticastProtocol::SERVER);

    // message
    message = protocol.message();
    QCOMPARE(message.value("id").toString(), databaseSettings.clientId());
    QCOMPARE(message.value("score").toUInt(), protocol.currentNode()->score());
    QCOMPARE(message.value("port").toInt(), 8081);
    QVERIFY(message.value("initialized").toUInt() > (uint) 0);
    QCOMPARE(message.value("status").toString(), QString("server"));

    // status 2
    QVERIFY(protocol.currentNode()->status() == MulticastProtocol::SERVER);

    // server
    QVERIFY(protocol.serverNode() == NULL);

    StubTime::cleanCurrentDateTime();
}

DECLARE_TEST(MulticastProtocolTests)

#include "multicastprotocoltests.moc"
