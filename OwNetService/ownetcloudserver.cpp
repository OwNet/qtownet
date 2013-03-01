#include "ownetcloudserver.h"

#include <QDebug>

OwNetCloudServer::OwNetCloudServer(QObject *parent) :
    QObject(parent)
{
}

void OwNetCloudServer::sendCrashReport(QString report)
{
    qDebug() << report;
}
