#include "testsocket.h"

#include <QBuffer>

TestSocket::TestSocket(QByteArray *data, QObject *parent) :
    QObject(parent), m_data(data)
{
}

QIODevice *TestSocket::ioDevice()
{
    QBuffer *buffer = new QBuffer(m_data, this);
    buffer->open(QBuffer::ReadWrite);
    return buffer;
}
