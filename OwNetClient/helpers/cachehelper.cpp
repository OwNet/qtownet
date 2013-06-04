#include "cachehelper.h"

#include <QHash>

bool CacheHelper::m_canUseDatabase = true;
bool CacheHelper::m_canWriteToCache = true;

CacheHelper::CacheHelper(QObject *parent) :
    QObject(parent)
{
}

uint CacheHelper::cacheId(const QString &url)
{
    QString lower = url.toLower();
    if (lower.endsWith('/'))
        lower.chop(1);
    return qHash(lower);
}
