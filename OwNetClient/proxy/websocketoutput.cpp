#include "websocketoutput.h"

#include "sockethandler.h"

#include <QFile>

WebSocketOutput::WebSocketOutput(QFile *file, QObject *parent) :
    QObject(parent),
    m_file(file),
    m_socketHandler(NULL)
{
    connect(this, SIGNAL(destroyed()), m_file, SLOT(deleteLater()));
}

WebSocketOutput::WebSocketOutput(SocketHandler *socketHandler, QObject *parent) :
    QObject(parent),
    m_file(NULL),
    m_socketHandler(socketHandler)
{
}

void WebSocketOutput::write(const QByteArray &bytes)
{
    if (m_file)
        m_file->write(bytes);
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
