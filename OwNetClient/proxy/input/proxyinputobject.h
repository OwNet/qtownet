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

    virtual void readRequest() = 0;
    virtual const QString httpStatusCode() = 0;
    virtual const QString httpStatusDescription() = 0;

    const QString &contentType() { return m_contentType; }

    const ListOfStringPairs &responseHeaders() { return m_responseHeaders; }

signals:
    void readyRead(QIODevice *ioDevice);
    void finished();
    
public slots:

protected:
    void addHeader(const QString &key, const QString &value);

    QString m_contentType;
    ProxyRequest *m_request;

private:
    ListOfStringPairs m_responseHeaders;
};

#endif // PROXYINPUTOBJECT_H
