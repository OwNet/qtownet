#include "messagehelper.h"

#include "applicationdatastorage.h"

#include <QMessageBox>
#include <QDebug>
#include <QFile>

QFile *MessageHelper::m_logFile = NULL;

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

    writeToLogFile(message);
}

void MessageHelper::writeToLogFile(const QString &log)
{
    if (!m_logFile) {
        m_logFile = new QFile(ApplicationDataStorage().appDataDirectory().absoluteFilePath("ownet.log"));
        m_logFile->open(QIODevice::Append | QIODevice::WriteOnly | QIODevice::Text);
    }

    QTextStream(m_logFile) << log << endl;
}
