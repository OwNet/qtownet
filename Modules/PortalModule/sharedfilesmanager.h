#ifndef SHAREDFILESHELPER_H
#define SHAREDFILESHELPER_H

#include <QObject>

class IProxyConnection;

class SharedFilesHelper : public QObject
{
    Q_OBJECT
public:
    explicit SharedFilesHelper(IProxyConnection *proxyConnection, QObject *parent = 0);

    QString url(const QString &fileName) const;
    void saveToDatabase(const QString &url, const QString &title, const QString &description);

private:
    IProxyConnection *m_proxyConnection;
};

#endif // SHAREDFILESHELPER_H
