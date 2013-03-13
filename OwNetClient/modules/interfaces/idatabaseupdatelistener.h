#ifndef IDATABASEUPDATELISTENER_H
#define IDATABASEUPDATELISTENER_H

#include <QString>

class IDatabaseUpdateQuery;

/**
 * @brief The IDatabaseUpdateListener class listens to updates in the listened table.
 * It is notified when a IDatabaseUpdateQuery is executed on the given table.
 */
class IDatabaseUpdateListener
{
public:
    virtual QString tableName() const = 0;

    virtual void tableUpdated(IDatabaseUpdateQuery *query) = 0;
};

#endif // IDATABASEUPDATELISTENER_H
