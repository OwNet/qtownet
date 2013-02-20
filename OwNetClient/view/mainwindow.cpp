#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "preferencesdialog.h"
#include "proxyconnection.h"
#include "session.h"
#include "proxyresponseoutputwriter.h"

#include <QSystemTrayIcon>
#include <QAction>
#include <QDesktopServices>
#include <QUrl>
#include <QShortcut>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->actionPreferences, SIGNAL(triggered()), this, SLOT(showPreferences()));
    connect(ui->btnPreferences, SIGNAL(clicked()), this, SLOT(showPreferences()));
    connect(ui->btnSync, SIGNAL(clicked()), this, SLOT(sync()));

    createTrayIcon();

    QShortcut *dumpSockets = new QShortcut(QKeySequence(tr("Ctrl+L", "Dump Open Socket")), this);
    dumpSockets->setContext(Qt::ApplicationShortcut);
    connect(dumpSockets, SIGNAL(activated()), this, SLOT(dumpOpenSockets()));

    QShortcut *dumpAvailableClients = new QShortcut(QKeySequence(tr("Ctrl+K", "Dump Available Socket")), this);
    dumpAvailableClients->setContext(Qt::ApplicationShortcut);
    connect(dumpAvailableClients, SIGNAL(activated()), this, SLOT(dumpAvailableClients()));
}

MainWindow::~MainWindow()
{
    delete ui;
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
