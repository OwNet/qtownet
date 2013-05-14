#include "proxydownloadtests.h"

#include "stubdatabase.h"
#include "proxydownload.h"
#include "artificialrequest.h"
#include "proxyrequest.h"
#include "proxyhandlersession.h"
#include "proxydownloadpart.h"
#include "proxydownloadstream.h"
#include "proxywebinputobject.h"

#include <QtTest>
#include <QFile>

ProxyDownloadTests::ProxyDownloadTests(QObject *parent) :
    QObject(parent)
{
}

void ProxyDownloadTests::init()
{
    StubDatabase::close();
    StubDatabase::init();
}

void ProxyDownloadTests::cleanupTestCase()
{
    StubDatabase::close();
}

void ProxyDownloadTests::testInputAndOutput()
{
    QVariantMap requestHeaders;
    requestHeaders.insert("User-Agent", "OwNetTests/1.0");
    ProxyRequest request(IRequest::GET, "http://www.fiit.stuba.sk", requestHeaders);
    ProxyHandlerSession session;
    ProxyDownload download(&request, &session);
    ProxyWebInputObject webInput(&request);
    download.setInputObject(&webInput);

    QByteArray ba = QString("Hello").toLatin1();
    QBuffer *buffer = new QBuffer(&ba);
    buffer->open(QBuffer::ReadOnly);
    download.readReply(buffer);
    QCOMPARE(download.downloadPart(download.registerReader())->stream()->stream()->readAll(), ba);

    QByteArray ba2 = QString("This is a test!").toLatin1();
    buffer = new QBuffer(&ba2);
    buffer->open(QBuffer::ReadOnly);
    download.readReply(buffer);
    int readerId = download.registerReader();
    QCOMPARE(download.downloadPart(readerId)->stream()->stream()->readAll(), ba);
    QCOMPARE(download.downloadPart(readerId)->stream()->stream()->readAll(), ba2);
}

void ProxyDownloadTests::testSimultaneousReadAndWrite()
{
    QFile file("test.txt");
    file.open(QFile::WriteOnly);
    file.write("HAHA! This is funny!");
    file.flush();
    QFile fileRead("test.txt");
    fileRead.open(QFile::ReadOnly);
    QCOMPARE(fileRead.readAll(), QByteArray("HAHA! This is funny!"));
    file.write("Soooo funny!");
    file.flush();
    QCOMPARE(fileRead.readAll(), QByteArray("Soooo funny!"));
}
