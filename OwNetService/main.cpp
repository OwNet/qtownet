#include <QApplication>
#include <QCoreApplication>

#include "settingsinitializer.h"
#include "ownetclient.h"
#include "ownetcloudserver.h"
#include "ownetupdater.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    (new SettingsInitializer())->init();

    OwNetCloudServer *cloudServer = new OwNetCloudServer();
    OwNetClient *client = new OwNetClient(cloudServer);
    client->start();

    OwNetUpdater *updater = new OwNetUpdater(client, cloudServer);
    updater->checkForUpdates();

    return a.exec();
}
