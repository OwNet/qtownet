#ifndef DATABASESELECTQUERYTESTS_H
#define DATABASESELECTQUERYTESTS_H

#include <QObject>

class DatabaseSelectQueryTests : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseSelectQueryTests(QObject *parent = 0);
    
private slots:
    void init();
    void cleanupTestCase();

    void testOrderBy();
};

#endif // DATABASESELECTQUERYTESTS_H
