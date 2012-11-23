#include "heartbeatjob.h"
#include "messagehelper.h"
#include "qjson/serializer.h"

#include <QUdpSocket>

HeartbeatJob::HeartbeatJob(QObject *parent)
    : Job(5 * 1000, parent)
{
}

void HeartbeatJob::execute()
{
    QUdpSocket *udpSocket = new QUdpSocket(this);
    QHostAddress groupAddress("225.225.225.225");

    QJson::Serializer serializer;
    QVariantMap map;
    map.insert("id", "aftghyrggf");
    map.insert("score", 3);
    QByteArray datagram = serializer.serialize(map);

    MessageHelper::debug(QString(datagram.data()));
    udpSocket->writeDatagram(datagram.data(), datagram.size(), groupAddress, 5081);
}
