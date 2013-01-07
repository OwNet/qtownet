#ifndef SYNCSERVERTESTS_H
#define SYNCSERVERTESTS_H

#include <QObject>

class SyncServerTests : public QObject
{
    Q_OBJECT
public:
    explicit SyncServerTests(QObject *parent = 0);

private slots:
    void initTestCase();
    void cleanupTestCase();

    void testClientRecordNumbers();
};

#endif // SYNCSERVERTESTS_H
