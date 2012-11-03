#ifndef PROXYINPUTOBJECT_H
#define PROXYINPUTOBJECT_H

#include <QObject>

#include "listofstringpairs.h"

class ProxyRequest;
class QIODevice;

class ProxyInputObject : public QObject
{
    Q_OBJECT

public:
    ProxyInputObject(ProxyRequest *request, QObject *parent = 0);

    void startDownload();

    const QString &httpStatusCode() { return m_httpStatusCode; }
    const QString &httpStatusDescription() { return m_httpStatusDescription; }
    const QString &contentType() { return m_contentType; }
    ListOfStringPairs &responseHeaders() { return m_responseHeaders; }
    ProxyRequest *request() { return m_request; }

signals:
    void readyRead(QIODevice *ioDevice);
    void finished();
    
public slots:

protected:
    void addHeader(const QString &key, const QString &value);
    virtual void readRequest() = 0;

    ProxyRequest *m_request;
    QString m_contentType;
    QString m_httpStatusCode;
    QString m_httpStatusDescription;
    bool m_downloadStarted;
    ListOfStringPairs m_responseHeaders;
};

#endif // PROXYINPUTOBJECT_H
