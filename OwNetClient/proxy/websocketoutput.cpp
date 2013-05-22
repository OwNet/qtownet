#include "websocketoutput.h"

#include "sockethandler.h"

#include <QFile>
#include <QDateTime>

WebSocketOutput::WebSocketOutput(QFile *file, QObject *parent) :
    QObject(parent),
    m_file(file),
    m_socketHandler(NULL),
    m_firstWrite(true)
{
    connect(this, SIGNAL(destroyed()), m_file, SLOT(deleteLater()));
}

WebSocketOutput::WebSocketOutput(SocketHandler *socketHandler, QObject *parent) :
    QObject(parent),
    m_file(NULL),
    m_socketHandler(socketHandler)
{
}

void WebSocketOutput::write(QByteArray &bytes)
{
    if (m_file) {
        if (m_firstWrite) {
            int pos = bytes.indexOf("\n");
            bytes.insert(pos+1, "X-OwNet-CachedOn: " + QDateTime::currentDateTimeUtc().toString(Qt::ISODate) + "\r\n");
            m_firstWrite = false;
        }
        m_file->write(bytes);
    }
    else if (m_socketHandler)
        m_socketHandler->write(bytes);
}

void WebSocketOutput::flush()
{
    if (m_file)
        m_file->flush();
}

void WebSocketOutput::close()
{
    if (m_file)
        m_file->close();
}

void WebSocketOutput::remove()
{
    if (m_file)
        m_file->remove();
}
