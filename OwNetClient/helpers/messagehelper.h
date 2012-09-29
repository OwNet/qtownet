#ifndef MESSAGEHELPER_H
#define MESSAGEHELPER_H

#include <QString>

class MessageHelper
{
public:
    MessageHelper();

    static void error(QString title, QString body);
    static void information(QString title, QString body);
    static void debug(QString message);
};

#endif // MESSAGEHELPER_H
