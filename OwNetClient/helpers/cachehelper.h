#ifndef CACHEHELPER_H
#define CACHEHELPER_H

#include <QObject>

class CacheHelper : public QObject
{
    Q_OBJECT
public:
    explicit CacheHelper(QObject *parent = 0);

    static uint cacheId(const QString &url);

    static bool canUseDatabase() { return m_canUseDatabase; }
    static void setCanUseDatabase(bool can) { m_canUseDatabase = can; }

    static bool canWriteToCache() { return m_canWriteToCache; }
    static void setCanWriteToCache(bool can) { m_canWriteToCache = can; }

private:
    static bool m_canUseDatabase;
    static bool m_canWriteToCache;
};

#endif // CACHEHELPER_H
