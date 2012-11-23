#ifndef SETTINGSINITIALIZER_H
#define SETTINGSINITIALIZER_H

#include <QObject>

class SettingsInitializer : public QObject
{
    Q_OBJECT
public:
    explicit SettingsInitializer(QObject *parent = 0);

    void init();
    
signals:
    
public slots:
    
};

#endif // SETTINGSINITIALIZER_H
