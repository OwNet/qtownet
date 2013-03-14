#ifndef PINGSERVER_H
#define PINGSERVER_H

#include <QObject>
#include <QVariantMap>

class MulticastProtocol;

class PingServer : public QObject
{
    Q_OBJECT
public:
    explicit PingServer(MulticastProtocol *multicastProtocol, QObject *parent = 0);

    void updateAvailableClients(const QVariantList &clients);
    void updateClient(const QVariantMap &client);
    QVariantList availableClients() const;

    MulticastProtocol *multicastProtocol() { return m_multicastProtocol; }

private:
    MulticastProtocol *m_multicastProtocol;
};

#endif // PINGSERVER_H
