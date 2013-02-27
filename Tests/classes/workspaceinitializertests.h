#ifndef WORKSPACEINITIALIZERTESTS_H
#define WORKSPACEINITIALIZERTESTS_H

#include <QObject>

class WorkspaceInitializerTests : public QObject
{
    Q_OBJECT
public:
    explicit WorkspaceInitializerTests(QObject *parent = 0);
    
private slots:
    void init();
    void cleanupTestCase();

    void testInit();
};

#endif // WORKSPACEINITIALIZERTESTS_H
