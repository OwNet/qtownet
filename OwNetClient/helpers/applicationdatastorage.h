#ifndef APPLICATIONDATASTORAGE_H
#define APPLICATIONDATASTORAGE_H

#include <QObject>
#include <QDir>

class ApplicationDataStorage : public QObject
{
    Q_OBJECT
public:
    explicit ApplicationDataStorage(QObject *parent = 0);

    QDir appDataDirectory() const;
    QDir appResourcesDirectory() const;

    QString databaseFilePath(const QString &workspaceId) const;
    
signals:
    
public slots:
    
};

#endif // APPLICATIONDATASTORAGE_H
