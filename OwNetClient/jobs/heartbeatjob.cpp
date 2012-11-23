#include "heartbeatjob.h"
#include "messagehelper.h"
#include "qjson/serializer.h"
#include "settings.h"

#include <QUdpSocket>

HeartbeatJob::HeartbeatJob(QObject *parent)
    : Job(5 * 1000, parent)
{
}

void HeartbeatJob::execute()
{
    QUdpSocket *udpSocket = new QUdpSocket(this);
    QHostAddress groupAddress(Settings().value("application/multicast_group_address", "227.227.227.1").toString());

    QJson::Serializer serializer;
    QVariantMap map;
    map.insert("id", "unique-client-id");
    map.insert("score", 1);
    QByteArray datagram = serializer.serialize(map);

    MessageHelper::debug(QString(datagram.data()));
    udpSocket->writeDatagram(datagram.data(), datagram.size(), groupAddress,
                             Settings().value("application/multicast_port", "8081").toInt());
}
