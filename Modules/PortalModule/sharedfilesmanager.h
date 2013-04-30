#ifndef SHAREDFILESMANAGER_H
#define SHAREDFILESMANAGER_H

#include <QObject>

class IProxyConnection;
class QFile;

class SharedFilesManager : public QObject
{
    Q_OBJECT
public:
    enum {
        ItemsPerPage = 10
    };
    enum Filter {
        MyFiles,
        AllFiles
    };

    explicit SharedFilesManager(IProxyConnection *proxyConnection, QObject *parent = 0);
    explicit SharedFilesManager(const QString &tempFileName, IProxyConnection *proxyConnection, QObject *parent = 0);

    void saveFileToCache();
    QVariantList listAvailableFiles(int page = 1, Filter filter = AllFiles);
    void removeFile(const QString &uid);
    int numberOfPages(Filter filter = AllFiles);

private:
    QByteArray getValueFor(QFile *tempFile, const QString &key, bool findFileName);
    QString createUrl(const QString &fileName);
    void saveToDatabase(const QString &url, qint64 size);

    QString m_tempFileName;
    QString m_fileName;
    QString m_title;
    QString m_description;
    QString m_contentType;
    IProxyConnection *m_proxyConnection;
};

#endif // SHAREDFILESMANAGER_H
