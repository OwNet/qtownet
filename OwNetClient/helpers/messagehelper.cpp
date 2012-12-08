#include "messagehelper.h"

#include "applicationdatastorage.h"

#ifndef TEST
#include <QMessageBox>
#endif

#include <QDebug>
#include <QFile>

QFile *MessageHelper::m_logFile = NULL;

MessageHelper::MessageHelper()
{
}

void MessageHelper::error(QString title, QString body)
{
#ifdef TEST
    qDebug() << title << body;
#else
    QMessageBox::critical(NULL, title, body);
#endif
}

void MessageHelper::information(QString title, QString body)
{
#ifdef TEST
    qDebug() << title << body;
#else
    QMessageBox::information(NULL, title, body);
#endif
}

void MessageHelper::debug(QString message)
{
    qDebug() << message;

    //writeToLogFile(message);
}

void MessageHelper::writeToLogFile(const QString &log)
{
    if (!m_logFile) {
        m_logFile = new QFile(ApplicationDataStorage().appDataDirectory().absoluteFilePath("ownet.log"));
        m_logFile->open(QIODevice::WriteOnly | QIODevice::Text);
    }

    QTextStream(m_logFile) << log << endl;
}
