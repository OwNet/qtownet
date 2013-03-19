#ifndef CACHEEXCEPTIONS_H
#define CACHEEXCEPTIONS_H

#include <QObject>
#include <QMap>
#include <QMutex>

#include "idatabaseupdatelistener.h"

/**
 * @brief Singleton, contains updated list of all cache exceptions
 */
class CacheExceptions : public QObject, public IDatabaseUpdateListener
{
    Q_OBJECT
public:
    CacheExceptions(QObject *parent = 0);

    QString tableName() const { return "cache_exceptions"; }
    void tableUpdated(IDatabaseUpdateQuery *query);

    bool containsExceptionFor(const QString &url) const;

private:
    void init();
    static QString getHostName(const QString &url);

    QMap<QString, bool> m_cacheExceptions;
};

#endif // CACHEEXCEPTIONS_H
