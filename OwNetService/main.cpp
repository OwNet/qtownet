#include <QApplication>
#include <QCoreApplication>

#include "settingsinitializer.h"
#include "ownetclient.h"
#include "ownetcloudserver.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    (new SettingsInitializer())->init();

    OwNetCloudServer *cloudServer = new OwNetCloudServer();

    OwNetClient *client = new OwNetClient(cloudServer);
    client->start();

    return a.exec();
}
