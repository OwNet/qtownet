#ifndef MULTIPARTFORMREADER_H
#define MULTIPARTFORMREADER_H

#include <QObject>

class IProxyConnection;

class SharedFilesManager : public QObject
{
    Q_OBJECT
public:
    explicit SharedFilesManager(const QString &tempFileName, IProxyConnection *proxyConnection, QObject *parent = 0);

    QString saveFileToCache(const QString &inputName);

private:
    QString createUrl(const QString &fileName);
    void saveToDatabase(const QString &url, const QString &contentType, qint64 size);

    QString m_tempFileName;
    QString m_title;
    QString m_description;
    IProxyConnection *m_proxyConnection;
};

#endif // MULTIPARTFORMREADER_H
