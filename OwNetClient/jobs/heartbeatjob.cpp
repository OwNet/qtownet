#include "heartbeatjob.h"
#include "messagehelper.h"
#include "qjson/serializer.h"

#include <QUdpSocket>

HeartbeatJob::HeartbeatJob(QHostAddress *groupAddress, int port, QObject *parent)
    : Job(5 * 1000, parent), m_groupAddress(groupAddress), m_port(port)
{
}

void HeartbeatJob::execute()
{
    QUdpSocket *udpSocket = new QUdpSocket(this);

    QJson::Serializer serializer;
    QVariantMap map;
    map.insert("id", "unique-client-id");
    map.insert("score", 1);
    QByteArray datagram = serializer.serialize(map);

    MessageHelper::debug(QString(datagram.data()));
    udpSocket->writeDatagram(datagram.data(), datagram.size(), *m_groupAddress, m_port);
}
