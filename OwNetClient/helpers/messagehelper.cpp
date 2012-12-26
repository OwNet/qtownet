#include "messagehelper.h"

#include "applicationdatastorage.h"

#ifndef TEST
#include <QMessageBox>
#endif

#include <QDebug>
#include <QFile>
#include <QQueue>

QFile *MessageHelper::m_logFile = NULL;
QQueue<QVariant> *MessageHelper::m_logsToBeWrittenToDisk = new QQueue<QVariant>();

MessageHelper::MessageHelper()
{
}

void MessageHelper::error(const QString &title, const QVariant &body)
{
#ifdef TEST
    qDebug() << title << body;
#else
    QMessageBox::critical(NULL, title, body.toString());
#endif
}

void MessageHelper::information(const QString &title, const QVariant &body)
{
#ifdef TEST
    qDebug() << title << body.toString();
#else
    QMessageBox::information(NULL, title, body.toString());
#endif
}

void MessageHelper::debug(const QVariant &message)
{
    qDebug() << message.toString();

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
        QTextStream(m_logFile) << m_logsToBeWrittenToDisk->dequeue().toString() << endl;
    } while (!m_logsToBeWrittenToDisk->isEmpty());
}

void MessageHelper::writeToLogFile(const QVariant &log)
{
    m_logsToBeWrittenToDisk->enqueue(log);
}
