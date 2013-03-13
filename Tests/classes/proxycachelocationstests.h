#ifndef PROXYCACHELOCATIONSTESTS_H
#define PROXYCACHELOCATIONSTESTS_H

#include <QObject>

class ProxyCacheLocationsTests : public QObject
{
    Q_OBJECT
public:
    explicit ProxyCacheLocationsTests(QObject *parent = 0);

private slots:
    void init();
    void cleanupTestCase();

    void testSortedLocations();
};

#endif // PROXYCACHELOCATIONSTESTS_H
