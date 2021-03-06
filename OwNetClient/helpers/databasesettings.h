#ifndef DATABASESETTINGS_H
#define DATABASESETTINGS_H

#include <QObject>

#include "idatabasesettings.h"

template<class Key, class T>
class QMap;

class DatabaseSettings : public QObject, public IDatabaseSettings
{
    Q_OBJECT
public:
    explicit DatabaseSettings(QObject *parent = 0);

    void setValue(const QString &key, const QString &value) const;
    QString value(const QString &key, const QString &defaultValue = QString()) const;

    bool hasClientId() const;
    QString clientId() const;
    void createClientId() const;

    int nextClientSyncRecordNumber();
    void saveLastClientSyncRecordNumber();

    void clearCache() const;

private:
    int clientSyncRecordNumber(bool increase);

    static int m_clientSyncRecordNumber;
    static int m_lastSavedClientSyncRecordNumber;

    static QMap<QString, QString> *m_cachedSettings;
    static int m_numClientIdsGenerated;
};

#endif // DATABASESETTINGS_H
