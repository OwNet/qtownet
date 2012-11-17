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

    loadSettings();
}

PreferencesDialog::~PreferencesDialog()
{
    delete ui;
}

void PreferencesDialog::savePreferences()
{
    Settings settings;

    QDir appData(ui->leAppDataPath->text());
    if (appData.exists())
        settings.setValue("application/data_folder_path", appData.absolutePath());
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

void PreferencesDialog::loadSettings()
{
    ApplicationDataStorage appData;
    ui->leAppDataPath->setText(appData.appDataDirectory().absolutePath());
}
