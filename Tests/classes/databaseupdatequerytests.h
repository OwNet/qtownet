#ifndef DATABASEUPDATEQUERYTESTS_H
#define DATABASEUPDATEQUERYTESTS_H

#include <QObject>

class DatabaseUpdateQueryTests : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseUpdateQueryTests(QObject *parent = 0);

private slots:
    void init();
    void cleanupTestCase();

    void testInsert();
    void testUpdate();
    void testDelete();
};

#endif // DATABASEUPDATEQUERYTESTS_H
