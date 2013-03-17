#ifndef CACHEHELPER_H
#define CACHEHELPER_H

#include <QObject>

class CacheHelper : public QObject
{
    Q_OBJECT
public:
    explicit CacheHelper(QObject *parent = 0);

    static uint cacheId(const QString &url);
};

#endif // CACHEHELPER_H
