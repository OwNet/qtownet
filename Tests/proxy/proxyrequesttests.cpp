#include "proxyrequesttests.h"

#include "stubdatabase.h"
#include "proxysocketrequest.h"

#include <QtTest>
#include <QBuffer>

ProxyRequestTests::ProxyRequestTests(QObject *parent) :
    QObject(parent)
{
}

void ProxyRequestTests::init()
{
    StubDatabase::close();
    StubDatabase::init();
}

void ProxyRequestTests::cleanupTestCase()
{
    StubDatabase::close();
}

void ProxyRequestTests::testReadFromSocket()
{
    QByteArray socketData;
    QTextStream os(&socketData, QIODevice::ReadWrite);
    os << "GET http://test.com/file.html HTTP/1.1\r\n";
    os << "User-Agent: OwNetTests/1.0\r\n";
    os << "\r\n";
    os.flush();

    QBuffer buffer(&socketData);
    buffer.open(QIODevice::ReadOnly);

    ProxySocketRequest request(&buffer);
    request.readFromSocket();
    QCOMPARE(IRequest::GET, request.requestType());
    QCOMPARE(QString("http://test.com/file.html"), request.url());
    QCOMPARE(1, request.requestHeaders().count());
    QCOMPARE(QString("OwNetTests/1.0"), request.requestHeaders().value("User-Agent").toString());
}
