#ifndef PROXYDOWNLOADSTREAM_H
#define PROXYDOWNLOADSTREAM_H

#include <QObject>

class QIODevice;

class ProxyDownloadStream : public QObject
{
    Q_OBJECT
public:
    explicit ProxyDownloadStream(QObject *parent = 0);

    void setStream(QIODevice *stream) { m_stream = stream; }
    QIODevice *stream() { return m_stream; }

private:
    QIODevice *m_stream;
};

#endif // PROXYDOWNLOADSTREAM_H
