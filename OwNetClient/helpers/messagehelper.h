#ifndef MESSAGEHELPER_H
#define MESSAGEHELPER_H

#include <QString>
#include <QVariant>

class QFile;
class QMutex;

class MessageHelper
{
public:
    MessageHelper();

    static void error(const QString &title, const QVariant &body);
    static void information(const QString &title, const QVariant &body);
    static void debug(const QVariant &message);

    static void writeLogFileToDisk();

private:
    static void writeToLogFile(const QVariant &log);

    static QFile *m_logFile;
    static QQueue<QVariant> *m_logsToBeWrittenToDisk;
};

#endif // MESSAGEHELPER_H
