#ifndef MESSAGEHELPER_H
#define MESSAGEHELPER_H

#include <QString>

class QMutex;
class QFile;

class MessageHelper
{
public:
    MessageHelper();

    static void error(QString title, QString body);
    static void information(QString title, QString body);
    static void debug(QString message);

private:
    static void writeToLogFile(const QString &log);
    static QFile *m_logFile;
};

#endif // MESSAGEHELPER_H
