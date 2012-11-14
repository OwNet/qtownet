#ifndef DATABASEUPDATE_H
#define DATABASEUPDATE_H

#include <QObject>
#include <QList>

#include "databaseupdatequery.h"

/**
 * @brief Container for insert, update and delete database queries.
 * Can be exported to JSON and stored in database journal.
 */
class DatabaseUpdate : public QObject
{
    Q_OBJECT

public:
    explicit DatabaseUpdate(bool sync = true, QObject *parent = 0);

    DatabaseUpdateQuery *createUpdateQuery(const QString &table, DatabaseUpdateQuery::EntryType type = DatabaseUpdateQuery::Insert);
    DatabaseUpdateQuery *createUpdateQuery(const QVariantMap &content);
    int execute();

private:
    QList<DatabaseUpdateQuery*> m_updateQueries;
    bool m_sync;
};

#endif // DATABASEUPDATE_H
