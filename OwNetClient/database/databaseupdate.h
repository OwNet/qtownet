#ifndef DATABASEUPDATE_H
#define DATABASEUPDATE_H

#include <QObject>
#include <QMap>

class QMutex;

/**
 * @brief Container for insert, update and delete database queries.
 * Can be exported to JSON and stored in database journal.
 */
class DatabaseUpdate : public QObject
{
    Q_OBJECT

public:
    static void saveLastRecordNumbers();
    static void setLastRecordNumber(int groupId, int clientRecNum);

private:
    static QMap<int, int> *m_lastRecordNumbers;
    static QMutex *m_lastRecordNumbersMutex;
};

#endif // DATABASEUPDATE_H
