#ifndef SYNCSERVERTESTS_H
#define SYNCSERVERTESTS_H

#include <QObject>

class SyncServerTests : public QObject
{
    Q_OBJECT
public:
    explicit SyncServerTests(QObject *parent = 0);

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    void testClientRecordNumbers();
};

#endif // SYNCSERVERTESTS_H
