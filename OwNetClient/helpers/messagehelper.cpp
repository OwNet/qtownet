#include "messagehelper.h"

#include <QMessageBox>
#include <QDebug>

MessageHelper::MessageHelper()
{
}

void MessageHelper::error(QString title, QString body)
{
    QMessageBox::critical(NULL, title, body);
}

void MessageHelper::information(QString title, QString body)
{
    QMessageBox::information(NULL, title, body);
}

void MessageHelper::debug(QString message)
{
    qDebug() << message;
}
