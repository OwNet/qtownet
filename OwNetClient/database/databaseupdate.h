#ifndef DATABASEUPDATE_H
#define DATABASEUPDATE_H

#include <QObject>
#include <QList>

#include "databaseupdatequery.h"

class DatabaseUpdate : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseUpdate(bool sync = true, QObject *parent = 0);

    DatabaseUpdateQuery *createUpdateQuery(const QString &table, DatabaseUpdateQuery::EntryType type = DatabaseUpdateQuery::Insert);
    void execute();

private:
    QList<DatabaseUpdateQuery*> m_updateQueries;
    bool m_sync;
};

#endif // DATABASEUPDATE_H
