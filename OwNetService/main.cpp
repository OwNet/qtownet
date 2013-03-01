#include <QApplication>

#include "settingsinitializer.h"

#include "settings.h"
#include <QDir>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("The Reconnected");
    QCoreApplication::setApplicationName("OwNet Client");

    (new SettingsInitializer())->init();
    
    QDir dir;
    dir.setPath(Settings().value("application/data_folder_path").toString());
    QFile file(dir.absoluteFilePath("ownet.pid"));
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    qDebug() << file.fileName();
    qDebug() << file.readAll().toLong();

    return a.exec();
}
