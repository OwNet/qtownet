#ifndef DATABASEUPDATEQUERY_H
#define DATABASEUPDATEQUERY_H

#include <QObject>
#include <QVariantMap>

class QSqlQuery;

class DatabaseUpdateQuery : public QObject
{
    Q_OBJECT
public:
    enum EntryType {
        Insert = 1,
        Update = 2,
        Detect = 3,
        Delete = 4
    };

    DatabaseUpdateQuery(const QString &table, EntryType type = Insert, QObject *parent = 0);
    DatabaseUpdateQuery(const QVariantMap &content, QObject *parent = 0);

    void setTable(const QString &table);
    void setType(EntryType type);
    void setReturningId(const QString &useColumnAs, const QVariantList &queries);
    void setColumnValue(const QString &name, const QVariant &value);
    void setTemporaryBinding(const QString &name, const QVariant &value);
    void setWhere(const QString &name, const QVariant &value);
    void setUpdateDates(bool setDates);
    QVariant bindingValue(const QString &name, const QVariant &value);
    void save();
    bool executeQuery();

    QString table() const;
    EntryType type() const;
    QVariantMap content() const;
    
private:
    QString timestamp() const;

    void appendWhere(QString &queryString, QSqlQuery &query);
    QVariantMap m_content;
    QVariantMap m_columns;
    QVariantMap m_wheres;
    QVariantMap m_temporaryBindings;
    bool m_needsSave;
};

#endif // DATABASEUPDATEQUERY_H
