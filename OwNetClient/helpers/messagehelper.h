#ifndef MESSAGEHELPER_H
#define MESSAGEHELPER_H

#include <QString>

class QFile;
class QMutex;

class MessageHelper
{
public:
    MessageHelper();

    static void error(QString title, QString body);
    static void information(QString title, QString body);
    static void debug(QString message);

    static void writeLogFileToDisk();

private:
    static void writeToLogFile(const QString &log);

    static QFile *m_logFile;
    static QQueue<QString> *m_logsToBeWrittenToDisk;
};

#endif // MESSAGEHELPER_H
