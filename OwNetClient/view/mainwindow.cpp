#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "preferencesdialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->actionPreferences, SIGNAL(triggered()), this, SLOT(showPreferences()));
    connect(ui->btnPreferences, SIGNAL(clicked()), this, SLOT(showPreferences()));
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
