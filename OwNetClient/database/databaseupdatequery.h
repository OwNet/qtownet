#ifndef DATABASEUPDATEQUERY_H
#define DATABASEUPDATEQUERY_H

#include "idatabaseupdatequery.h"

#include <QObject>
#include <QVariantMap>

class QSqlQuery;

/**
 * @brief Database query to update, insert or delete data.
 * Can be exported to JSON.
 */
class DatabaseUpdateQuery : public QObject, public IDatabaseUpdateQuery
{
    Q_OBJECT
public:
    DatabaseUpdateQuery(const QString &table, EntryType type = Insert, QObject *parent = 0);
    DatabaseUpdateQuery(const QVariantMap &content, QObject *parent = 0);

    void setTable(const QString &table);
    void setType(EntryType type);
    void setReturningId(const QString &useColumnAs, const QVariantList &queries);
    void setColumnValue(const QString &name, const QVariant &value);
    void setWhere(const QString &name, const QVariant &value);
    void setUpdateDates(bool setDates);
    void setUpdateDates(IDatabaseUpdateQuery::UpdateDates updateDates);
    bool executeQuery();

    QString table() const;
    EntryType type() const;
    QVariantMap content() const;
    
private:
    void save();
    void setTemporaryBinding(const QString &name, const QVariant &value);
    QVariant bindingValue(const QString &name, const QVariant &value) const;
    QString timestamp() const;

    void appendWhere(QString &queryString, QSqlQuery &query);
    QVariantMap m_content;
    QVariantMap m_columns;
    QVariantMap m_wheres;
    QVariantMap m_temporaryBindings;
    bool m_needsSave;
};

#endif // DATABASEUPDATEQUERY_H
