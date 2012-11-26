#ifndef STUBTIME_H
#define STUBTIME_H

#include <QDateTime>;

class StubTime
{

public:
    static void cleanCurrentDateTime();
    static void setCurrentDateTime(QDateTime);
    static QDateTime currentDateTime();
    static void addSecs(int);

private:
    static QDateTime m_currentDateTime;
};

#endif // STUBTIME_H
