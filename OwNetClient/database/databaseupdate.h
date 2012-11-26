#ifndef DATABASEUPDATE_H
#define DATABASEUPDATE_H

#include "idatabaseupdate.h"
#include "databaseupdatequery.h"

#include <QObject>
#include <QList>

/**
 * @brief Container for insert, update and delete database queries.
 * Can be exported to JSON and stored in database journal.
 */
class DatabaseUpdate : public QObject, public IDatabaseUpdate
{
    Q_OBJECT

public:
    DatabaseUpdate(QObject *parent = 0);
    DatabaseUpdate(bool sync, QObject *parent = 0);

    void setSync(bool sync) { m_sync = sync; }
    IDatabaseUpdateQuery *createUpdateQuery(const QString &table, IDatabaseUpdateQuery::EntryType type = IDatabaseUpdateQuery::Insert);
    IDatabaseUpdateQuery *createUpdateQuery(const QVariantMap &content);
    int execute();

private:
    QList<DatabaseUpdateQuery*> m_updateQueries;
    bool m_sync;
};

#endif // DATABASEUPDATE_H
