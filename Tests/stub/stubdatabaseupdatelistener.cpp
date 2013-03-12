#include "stubdatabaseupdatelistener.h"

StubDatabaseUpdateListener::StubDatabaseUpdateListener(QString tableName, QObject *parent) :
    QObject(parent),
    m_tableName(tableName),
    m_query(NULL)
{
}

void StubDatabaseUpdateListener::tableUpdated(IDatabaseUpdateQuery *query)
{
    m_query = query;
}
