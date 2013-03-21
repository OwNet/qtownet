#ifndef CACHEEXCEPTIONSMANAGERTESTS_H
#define CACHEEXCEPTIONSMANAGERTESTS_H

#include <QObject>

class CacheExceptionsManagerTests : public QObject
{
    Q_OBJECT
public:
    explicit CacheExceptionsManagerTests(QObject *parent = 0);

private slots:
    void init();
    void cleanupTestCase();

    void testGetHostName();
};

#endif // CACHEEXCEPTIONSMANAGERTESTS_H
