#include "uniqueidhelper.h"

#include <QUuid>

UniqueIdHelper::UniqueIdHelper(QObject *parent) :
    QObject(parent)
{
}

QString UniqueIdHelper::generate()
{
    return QUuid::createUuid().toString();
}
