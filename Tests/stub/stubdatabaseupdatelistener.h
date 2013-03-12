#ifndef STUBDATABASEUPDATELISTENER_H
#define STUBDATABASEUPDATELISTENER_H

#include "idatabaseupdatelistener.h"

#include <QObject>

class StubDatabaseUpdateListener : public QObject, public IDatabaseUpdateListener
{
    Q_OBJECT
public:
    explicit StubDatabaseUpdateListener(QString tableName, QObject *parent = 0);

    QString tableName() const { return m_tableName; }
    void tableUpdated(IDatabaseUpdateQuery *query);

    IDatabaseUpdateQuery *updateQuery() { return m_query; }
    void clear() { m_query = NULL; }

private:
    QString m_tableName;
    IDatabaseUpdateQuery *m_query;
};

#endif // STUBDATABASEUPDATELISTENER_H
