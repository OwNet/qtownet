#ifndef CACHEEXCEPTIONMANAGER_H
#define CACHEEXCEPTIONMANAGER_H

#include <QObject>

class IProxyConnection;

class CacheExceptionsManager : public QObject
{
    Q_OBJECT
public:
    explicit CacheExceptionsManager(IProxyConnection *proxyConnection, QObject *parent = 0);

    void addException(const QString &url);
    void removeException(const QString &url);
    bool isException(const QString &url);

    static QString getHostName(const QString &url);

private:
    IProxyConnection *m_proxyConnection;
};

#endif // CACHEEXCEPTIONMANAGER_H
