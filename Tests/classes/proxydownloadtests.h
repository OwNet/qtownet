#ifndef PROXYDOWNLOADTESTS_H
#define PROXYDOWNLOADTESTS_H

#include <QObject>

class ProxyDownloadTests : public QObject
{
    Q_OBJECT
public:
    explicit ProxyDownloadTests(QObject *parent = 0);

private slots:
    void init();
    void cleanupTestCase();

    void testInputAndOutput();
    void testSimultaneousReadAndWrite();
};

#endif // PROXYDOWNLOADTESTS_H
