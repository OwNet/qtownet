#ifndef ISETTINGS_H
#define ISETTINGS_H

#include <QSettings>

class ISettings : public QSettings
{
public:
    ISettings(QObject *parent = NULL) : QSettings(parent) {}

    virtual int listenPort() const = 0;
};

#endif // ISETTINGS_H
