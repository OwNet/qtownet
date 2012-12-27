#include "preferencesdialog.h"
#include "ui_preferencesdialog.h"

#include "applicationdatastorage.h"
#include "settings.h"

#include <QFileDialog>

PreferencesDialog::PreferencesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PreferencesDialog)
{
    ui->setupUi(this);

    connect(this, SIGNAL(accepted()), this, SLOT(savePreferences()));
    connect(this->ui->btnAppDataBrowse, SIGNAL(clicked()), this, SLOT(browseAppData()));
    connect(this->ui->btnAppResourcesBrowse, SIGNAL(clicked()), this, SLOT(browseAppResources()));

    loadSettings();
}

PreferencesDialog::~PreferencesDialog()
{
    delete ui;
}

void PreferencesDialog::savePreferences()
{
    Settings settings;

    QDir dir(ui->leAppDataPath->text());
    if (dir.exists() || dir.mkpath(dir.absolutePath()))
        settings.setValue("application/data_folder_path", dir.absolutePath());

    dir.setPath(ui->leAppResourcesPath->text());
    if (dir.exists() || dir.mkpath(dir.absolutePath()))
        settings.setValue("application/resources_folder_path", dir.absolutePath());
}

void PreferencesDialog::browseAppData()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    QDir::homePath(),
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    if (!dir.isNull())
        ui->leAppDataPath->setText(dir);
}

void PreferencesDialog::browseAppResources()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    QDir::homePath(),
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    if (!dir.isNull())
        ui->leAppResourcesPath->setText(dir);
}

void PreferencesDialog::loadSettings()
{
    ApplicationDataStorage appData;
    ui->leAppDataPath->setText(appData.appDataDirectory().absolutePath());
    ui->leAppResourcesPath->setText(appData.appResourcesDirectory().absolutePath());
}
