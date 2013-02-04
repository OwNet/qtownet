#ifndef PROXYREQUESTTESTS_H
#define PROXYREQUESTTESTS_H

#include <QObject>

class ProxyRequestTests : public QObject
{
    Q_OBJECT
public:
    explicit ProxyRequestTests(QObject *parent = 0);

private slots:
    void init();
    void cleanupTestCase();

    void testReadFromSocket();
};

#endif // PROXYREQUESTTESTS_H
