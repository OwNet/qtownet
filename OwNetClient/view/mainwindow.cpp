#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "preferencesdialog.h"
#include "proxyconnection.h"
#include "session.h"
#include "proxyresponseoutputwriter.h"
#include "workspaceitem.h"
#include "workspacehelper.h"
#include "settings.h"

#include <QSystemTrayIcon>
#include <QAction>
#include <QDesktopServices>
#include <QUrl>
#include <QShortcut>
#include <QDebug>
#include <QDateTime>
#include <QInputDialog>
#include <QDir>

MainWindow *MainWindow::m_instance = NULL;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->actionPreferences, SIGNAL(triggered()), this, SLOT(showPreferences()));
    connect(ui->btnPreferences, SIGNAL(clicked()), this, SLOT(showPreferences()));
    connect(ui->btnSync, SIGNAL(clicked()), this, SLOT(sync()));
    connect(ui->btnEditWorkspaceName, SIGNAL(clicked()), this, SLOT(editWorkspaceName()));
    connect(ui->btnNewWorkspace, SIGNAL(clicked()), this, SLOT(newWorkspace()));

#if defined(Q_WS_WIN) || defined(Q_WS_MAC)
    createTrayIcon();
#endif

    QShortcut *dumpSockets = new QShortcut(QKeySequence(tr("Ctrl+L", "Dump Open Socket")), this);
    dumpSockets->setContext(Qt::ApplicationShortcut);
    connect(dumpSockets, SIGNAL(activated()), this, SLOT(dumpOpenSockets()));

    QShortcut *dumpAvailableClients = new QShortcut(QKeySequence(tr("Ctrl+K", "Dump Available Socket")), this);
    dumpAvailableClients->setContext(Qt::ApplicationShortcut);
    connect(dumpAvailableClients, SIGNAL(activated()), this, SLOT(dumpAvailableClients()));

    ui->treeWidget->header()->setSectionResizeMode(QHeaderView::ResizeToContents);

    Settings settings;
    settings.beginGroup("workspaces");
    foreach (QString id, settings.childKeys()) {
        WorkspaceItem *item = new WorkspaceItem(id, settings.value(id).toString(), ui->treeWidget, this);
        item->setJoined(true);
        m_workspaceItems.insert(id, item);
    }

    MainWindow::m_instance = this;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::update()
{
    ui->lblWorkspaceName->setText(WorkspaceHelper::currentWorkspaceName());

    Session session;
    ui->btnEditWorkspaceName->setEnabled(session.isServer());

    QVariantMap workspaces = session.value("workspaces").toMap();
    foreach (QString id, workspaces.keys()) {
        QVariantMap workspace = workspaces.value(id).toMap();
        if (workspace.value("last_seen").toDateTime().addSecs(15) > QDateTime::currentDateTime()) {
            QString name = workspace.value("name").toString();
            if (!m_workspaceItems.contains(id))
                m_workspaceItems.insert(id, new WorkspaceItem(id, name, ui->treeWidget, this));

            WorkspaceItem *item = m_workspaceItems.value(id);
            if (item->name() != name)
                item->setName(name);
            item->setActive(true);
        } else if (m_workspaceItems.contains(id)) {
            m_workspaceItems.value(id)->setActive(false);
        }
    }
}

void MainWindow::showPreferences()
{
    PreferencesDialog *dialog = new PreferencesDialog(this);
    dialog->show();
}

void MainWindow::openMyOwNet()
{
    QDesktopServices::openUrl(QUrl("http://my.ownet"));
}

void MainWindow::dumpOpenSockets()
{
    qDebug() << tr("+++ Open Sockets +++");

    foreach (QString url, ProxyResponseOutputWriter::dumpOpenRequests())
        qDebug() << url;

    qDebug() << tr("--- Open Sockets ---");
}

void MainWindow::dumpAvailableClients()
{
    qDebug() << tr("+++ Available Clients +++");

    Session session;
    qDebug() << "Server ID: " << session.serverId();

    QVariantMap availableClients = session.availableClients();
    foreach (QString clientId, availableClients.keys())
        qDebug() << "ID: " << clientId << ", IP: " << availableClients.value(clientId).toString();

    qDebug() << tr("--- Available Clients ---");
}

void MainWindow::sync()
{
    ProxyConnection connection;
    connection.callModule(connection.createRequest(IRequest::GET, "sync", "now", &connection));
}

void MainWindow::editWorkspaceName()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("Change workspace name"),
                                         tr("New name:"), QLineEdit::Normal,
                                         WorkspaceHelper::currentWorkspaceName(), &ok);
    if (ok && !text.isEmpty()) {
        WorkspaceHelper::setCurrentWorkspaceName(text);
        QString id = WorkspaceHelper::currentWorkspaceId();
        if (m_workspaceItems.contains(id))
            m_workspaceItems.value(id)->setName(text);
    }
}

void MainWindow::newWorkspace()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("Enter the name of the new workspace"),
                                         tr("Name of the new workspace:"), QLineEdit::Normal,
                                         "", &ok);
    if (ok && !text.isEmpty())
        WorkspaceHelper::createAndLoadNewWorkspace(text);
}

void MainWindow::createTrayIcon()
{
    QMenu *trayIconMenu = new QMenu(this);
    QAction *myOwNetAction = new QAction(tr("Open my.ownet"), trayIconMenu);
    trayIconMenu->addAction(myOwNetAction);
    trayIconMenu->addSeparator();
    QAction *showWindowAction = new QAction(tr("Show"), trayIconMenu);
    trayIconMenu->addAction(showWindowAction);
    QAction *hideWindowAction = new QAction(tr("Hide"), trayIconMenu);
    trayIconMenu->addAction(hideWindowAction);
    trayIconMenu->addSeparator();
    QAction *quitAction = new QAction(tr("Quit"), trayIconMenu);
    trayIconMenu->addAction(quitAction);

    connect(showWindowAction, SIGNAL(triggered()), this, SLOT(show()));
    connect(hideWindowAction, SIGNAL(triggered()), this, SLOT(hide()));
    connect(quitAction, SIGNAL(triggered()), this, SLOT(close()));
    connect(myOwNetAction, SIGNAL(triggered()), this, SLOT(openMyOwNet()));

    QSystemTrayIcon *trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/images/icon32"));
    trayIcon->setToolTip("OwNet");
    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->show();
}
