#include "settings.h"

int Settings::m_temporaryListenPort = -1;

Settings::Settings(QObject *parent)
    : ISettings(parent)
{
}

int Settings::listenPort() const
{
    if (m_temporaryListenPort > 0)
        return m_temporaryListenPort;
    return value("application/listen_port", 8081).toInt();
}
