#ifndef IDATABASESETTINGS_H
#define IDATABASESETTINGS_H

#include <QString>

class IDatabaseSettings
{
public:
    virtual void setValue(const QString &key, const QString &value) = 0;
    virtual QString value(const QString &key, const QString &defaultValue = QString()) const = 0;
};

#endif // IDATABASESETTINGS_H
