#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QtCore/QCoreApplication>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QVariant>
#include <QMapIterator>

#include "autotest.h"
#include "modulehelpers.h"
#include "stubdatabase.h"

#include "artificialrequest.h"
#include "proxyconnection.h"
#include "irequest.h"
#include "requestrouter.h"
#include "imodule.h"
#include "irestservice.h"
#include "variantmap.h"
#include "isession.h"


class UsersServiceTests : public QObject
{
    Q_OBJECT

public:
    UsersServiceTests();

private:    
    IModule *m_module;
    ProxyConnection *m_proxyConnection;

    QVariantMap createUser(bool insert = true);
    QSqlRecord getUser();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    void testCreate();
    void testCreate_data();

    void testIndex();
    void testIndex_data();

    void testShow();
    void testShow_data();

    void testEdit();
    void testEdit_data();

    void testDel();
    void testDel_data();

};

UsersServiceTests::UsersServiceTests()
{
}

QVariantMap UsersServiceTests::createUser(bool insert)
{
    VariantMap user;

    user.set("id",1)
        ->set("login","login")
        ->set("password", "pfm")
        ->set("first_name", "first")
        ->set("last_name", "last")
        ->set("role_id", 1);

    if (insert) {
        QSqlQuery q;
        q.prepare("INSERT INTO users(id,login,password,first_name,last_name,role_id) VALUES(:id,:login,:password,:first_name,:last_name,:role_id)");

        for (auto it = user.begin(); it!=user.end(); it++)
            q.bindValue(":"+it.key(), it.value().toString() );

        if (! q.exec())
            qCritical() << "INSERT INTO users FAILED: " + q.lastError().text();
    }

    return user;
}

QSqlRecord UsersServiceTests::getUser()
{
    QSqlQuery q;
    q.exec("SELECT * FROM users LIMIT 1");

    if (q.first())
        return q.record();
    else
        qCritical("Failed: testCreate");
}



void UsersServiceTests::initTestCase()
{
    StubDatabase::init();

    m_module = ModuleHelpers::loadModule("ownet_portalmodule");

    // initialize module
    m_proxyConnection = new ProxyConnection();
    m_module->init(m_proxyConnection);    
}

void UsersServiceTests::cleanupTestCase()
{
    StubDatabase::close();
}

// Test Create
void UsersServiceTests::testCreate()
{
    StubDatabase::init();

    QVariantMap user = createUser(false);
    user.take("id");

    IRequest* req = m_proxyConnection->createRequest(IRequest::POST,"users");
    req->setPostBody(user);

    IResponse* resp = m_proxyConnection->callModule(req);

    QFETCH(int, http_create);
    QCOMPARE(resp->status(), http_create);

    auto row = getUser();

    QFETCH(QString, login);
    QCOMPARE(row.value("login").toString(), login);
}

void UsersServiceTests::testCreate_data()
{
    QTest::addColumn<int>("http_create");
    QTest::addColumn<QString>("login");
    QTest::newRow("test") << (int) IResponse::CREATED << "login";
}

// Test Index
void UsersServiceTests::testIndex()
{
    StubDatabase::init();
    createUser();

    IRequest* req = m_proxyConnection->createRequest(IRequest::GET,"users");
    IResponse* resp = m_proxyConnection->callModule(req);

    QFETCH(int, http_ok);
    QCOMPARE(resp->status(), http_ok);

    QFETCH(int, type);
    QCOMPARE((int) resp->body().type(), type);
}

void UsersServiceTests::testIndex_data()
{
    QTest::addColumn<int>("http_ok");
    QTest::addColumn<int>("type");
    QTest::newRow("status") << (int) IResponse::OK << (int) QMetaType::QVariantList;
}

// Test Show
void UsersServiceTests::testShow()
{
    StubDatabase::init();
    QVariantMap user = createUser();
    int id = user.value("id").toInt();

    IRequest* req = m_proxyConnection->createRequest(IRequest::GET,"users", id);
    IResponse* resp = m_proxyConnection->callModule(req);

    QFETCH(int, http_ok);
    QCOMPARE(resp->status(), http_ok);

    QFETCH(QString, login);
    QCOMPARE(resp->body().toMap().value("login").toString(), login);
}

void UsersServiceTests::testShow_data()
{
    QTest::addColumn<int>("http_ok");
    QTest::addColumn<QString>("login");
    QTest::newRow("test") << (int) IResponse::OK << "login";
}


// Test Edit
void UsersServiceTests::testEdit()
{
    StubDatabase::init();
    QVariantMap user = createUser();
    int id = user.value("id").toInt();

    QFETCH(QString, login);
    user.insert("login",login);

    QObject parent;
    m_proxyConnection->session(&parent)->setValue("logged", id);

    IRequest* req = m_proxyConnection->createRequest(IRequest::PUT,"users", id);
    req->setPostBody(user);

    IResponse* resp = m_proxyConnection->callModule(req);

    QFETCH(int, http_ok);
    QCOMPARE(resp->status(), http_ok);

    auto row = getUser();

    QCOMPARE(row.value("login").toString(), login);

}

void UsersServiceTests::testEdit_data()
{
    QTest::addColumn<int>("http_ok");
    QTest::addColumn<QString>("login");
    QTest::newRow("status") << (int) IResponse::OK << "changed";
}

// Test Delete
void UsersServiceTests::testDel()
{
    StubDatabase::init();
    QVariantMap user = createUser();
    int id = user.value("id").toInt();

    QObject parent;
    m_proxyConnection->session(&parent)->setValue("logged", id);

    IRequest* req = m_proxyConnection->createRequest(IRequest::DELETE,"users", id);
    IResponse* resp = m_proxyConnection->callModule(req);

    QFETCH(int, http_no_content);
    QCOMPARE(resp->status(), http_no_content);

    QSqlQuery q;
    q.exec("SELECT count(*) FROM users");

    QFETCH(int, count);
    QCOMPARE( q.record().value(0).toInt(), count);

}

void UsersServiceTests::testDel_data()
{
    QTest::addColumn<int>("http_no_content");
    QTest::addColumn<int>("count");
    QTest::newRow("status") << (int) IResponse::NO_CONTENT << 0;
}

DECLARE_TEST(UsersServiceTests)

#include "usersservicetests.moc"
