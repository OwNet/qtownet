#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>

class WorkspaceItem;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    static MainWindow *instance() { return m_instance; }
    void update();

private slots:
    void showPreferences();
    void openMyOwNet();
    void dumpOpenSockets();
    void dumpAvailableClients();
    void sync();
    void crash();
    void editWorkspaceName();
    void newWorkspace();
    void updateContent();

signals:
    void sigUpdate();
    
private:
    void createTrayIcon();

    QMap<QString, WorkspaceItem*> m_workspaceItems;

    static MainWindow *m_instance;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
