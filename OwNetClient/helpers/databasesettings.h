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

    void setValue(const QString &key, const QString &value);
    QString value(const QString &key, const QString &defaultValue = QString()) const;

    bool hasClientId() const;
    int clientId() const;
    void createClientId();

    int nextClientSyncRecordNumber();

private:
    static QMap<QString, QString> *m_cachedSettings;
};

#endif // DATABASESETTINGS_H
