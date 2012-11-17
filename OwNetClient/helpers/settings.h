#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>

class Settings : public QSettings
{
public:
    Settings(QObject *parent = NULL);

    void setValue(const QString &key, const QVariant &value);
    QVariant value(const QString &key, const QVariant &defaultValue = QVariant()) const;

private:
    static QMap<QString ,QVariant> *m_cachedSettings;
};

#endif // SETTINGS_H
