#ifndef WORKSPACEINITIALIZER_H
#define WORKSPACEINITIALIZER_H

#include <QObject>

class WorkspaceInitializer : public QObject
{
    Q_OBJECT
public:
    explicit WorkspaceInitializer(QObject *parent = 0);

    void init();
};

#endif // WORKSPACEINITIALIZER_H
