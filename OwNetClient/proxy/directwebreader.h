#ifndef DIRECTWEBREADER_H
#define DIRECTWEBREADER_H

#include "iwebdownload.h"

class SocketHandler;
class ProxyRequest;
class QIODevice;
class ProxyHandlerSession;

class DirectWebReader : public QObject, public IWebDownload
{
    Q_OBJECT
public:
    explicit DirectWebReader(SocketHandler *socketHandler, ProxyRequest *request, ProxyHandlerSession *session, QObject *parent = 0);
    
    void read();

    void downloadFailed();
    void downloadFinished(qint64 size);

private:
    void close();

    ProxyRequest *m_request;
    SocketHandler *m_socketHandler;
    ProxyHandlerSession *m_session;
    int m_dependentObjectId;
};

#endif // DIRECTWEBREADER_H
