#ifndef SETTINGS_H
#define SETTINGS_H

#include "isettings.h"

class Settings : public ISettings
{
public:
    Settings(QObject *parent = NULL);

    int listenPort() const;
    static void setTemporaryListenPort(int port) { m_temporaryListenPort = port; }

private:
    static int m_temporaryListenPort;
};

#endif // SETTINGS_H
