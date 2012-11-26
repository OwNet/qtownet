#ifndef IDATABASEUPDATEQUERY_H
#define IDATABASEUPDATEQUERY_H

#include <QString>
#include <QVariant>

class IDatabaseUpdateQuery
{
public:
    enum EntryType {
        Insert = 1,
        Update = 2,
        Detect = 3,
        Delete = 4
    };

    virtual void setReturningId(const QString &useColumnAs, const QVariantList &queries) = 0;
    virtual void setColumnValue(const QString &name, const QVariant &value) = 0;
    virtual void setWhere(const QString &name, const QVariant &value) = 0;
    virtual void setUpdateDates(bool setDates) = 0;

    virtual QString table() const = 0;
    virtual EntryType type() const = 0;
    virtual QVariantMap content() const = 0;
};

#endif // IDATABASEUPDATEQUERY_H
