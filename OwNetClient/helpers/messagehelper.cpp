#include "messagehelper.h"

#include "applicationdatastorage.h"

#ifndef TEST
#include <QMessageBox>
#endif

#include <QDebug>
#include <QFile>
#include <QQueue>

QFile *MessageHelper::m_logFile = NULL;
QQueue<QString> *MessageHelper::m_logsToBeWrittenToDisk = new QQueue<QString>();

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

    writeToLogFile(message);
}

void MessageHelper::writeLogFileToDisk()
{
    if (m_logsToBeWrittenToDisk->isEmpty())
        return;

    if (!m_logFile) {
        m_logFile = new QFile(ApplicationDataStorage().appDataDirectory().absoluteFilePath("ownet.log"));
        m_logFile->open(QIODevice::WriteOnly | QIODevice::Text);
    }

    do {
        QTextStream(m_logFile) << m_logsToBeWrittenToDisk->dequeue() << endl;
    } while (!m_logsToBeWrittenToDisk->isEmpty());
}

void MessageHelper::writeToLogFile(const QString &log)
{
    m_logsToBeWrittenToDisk->enqueue(log);
}
