#include <QApplication>
#include <QCoreApplication>

#include "settingsinitializer.h"
#include "ownetclient.h"

/*
#include "settings.h"
#include <QDir>
#include <QDebug>
*/

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    (new SettingsInitializer())->init();

    // read pidfile
    /*
    QDir dir;
    dir.setPath(Settings().value("application/data_folder_path").toString());
    QFile file(dir.absoluteFilePath("ownet.pid"));
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    qDebug() << file.fileName();
    qDebug() << file.readAll().toLong();
    */

    OwNetClient *client = new OwNetClient();
    client->start();

    return a.exec();
}
