#ifndef WORKSPACEHELPERTESTS_H
#define WORKSPACEHELPERTESTS_H

#include <QObject>

class WorkspaceHelperTests : public QObject
{
    Q_OBJECT
public:
    explicit WorkspaceHelperTests(QObject *parent = 0);
    
private slots:
    void init();
    void cleanupTestCase();

    void testCurrentWorkspace();
};

#endif // WORKSPACEHELPERTESTS_H
