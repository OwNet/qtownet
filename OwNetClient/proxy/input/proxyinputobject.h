#ifndef PROXYINPUTOBJECT_H
#define PROXYINPUTOBJECT_H

#include <QObject>

#include "variantmap.h"

class ProxyRequest;
class QIODevice;

/**
 * @brief Input source that downloads the given request and returns data to the ProxyDownload object.
 */
class ProxyInputObject : public QObject
{
    Q_OBJECT

public:
    enum InputType {
        Web,
        Cache,
        LocalFile,
        LocalService
    };

    ProxyInputObject(ProxyRequest *request, QObject *parent = 0);

    void startDownload();

    QString httpStatusCode() const { return m_httpStatusCode; }
    void setHttpStatusCode(int code);
    QString httpStatusDescription() const { return m_httpStatusDescription.isNull() ? QString() : m_httpStatusDescription; }
    void setHttpStatusDescription(const QString &description);
    virtual QString contentType() const;
    virtual void setContentType(const QString &value);
    VariantMap responseHeaders() const { return m_responseHeaders; }
    ProxyRequest *request() const { return m_request; }

    virtual InputType inputType() const = 0;

signals:
    void readyRead(QIODevice *ioDevice);
    void finished();
    void failed();

public slots:

protected:
    void addHeader(const QString &key, const QString &value);
    virtual void readRequest() = 0;

    ProxyRequest *m_request;
    QString m_httpStatusCode;
    QString m_httpStatusDescription;
    bool m_downloadStarted;
    VariantMap m_responseHeaders;
};

#endif // PROXYINPUTOBJECT_H
