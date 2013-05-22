#ifndef WEBSOCKETOUTPUT_H
#define WEBSOCKETOUTPUT_H

#include <QObject>

class QFile;
class SocketHandler;

class WebSocketOutput : public QObject
{
    Q_OBJECT
public:
    explicit WebSocketOutput(QFile *file, QObject *parent = 0);
    explicit WebSocketOutput(SocketHandler *socketHandler, QObject *parent = 0);

    void write(QByteArray &bytes);
    void flush();
    void close();
    void remove();
    
private:
    QFile *m_file;
    SocketHandler *m_socketHandler;    
    bool m_firstWrite;
};

#endif // WEBSOCKETOUTPUT_H
