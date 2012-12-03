#ifndef IDATABASESELECTQUERYWHERE_H
#define IDATABASESELECTQUERYWHERE_H

#include <QObject>
#include <QString>

class QSqlQuery;

class IDatabaseSelectQueryWhere : public QObject
{
    Q_OBJECT
public:
    IDatabaseSelectQueryWhere(QObject *parent = 0) : QObject(parent) {}

    virtual QString toString() = 0;
    virtual void bindValue(QSqlQuery *query) const = 0;
};

#endif // IDATABASESELECTQUERYWHERE_H
