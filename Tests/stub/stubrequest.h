#ifndef STUBREQUEST_H
#define STUBREQUEST_H

#include <QObject>

#include "../OwNetClient/modules/interfaces/irequest.h"

class StubRequest : public QObject, public IRequest
{
    Q_OBJECT
public:
    explicit StubRequest(QObject *parent = 0);

    QVariantMap postBodyFromJson() const;
    QMap<QString, QString> postBodyFromForm() const;

    IRequest::RequestType requestType() const;
    QVariantMap requestHeaders() const;

    QUrl qUrl() const;
    QString url() const;
    QString requestContentType(const QString &defaultContentType = "", const QString &extension = "") const;
    QString relativeUrl() const;
    QString action() const;
    QString module() const;
    QString subDomain() const;

    int id() const;
    QString parameterValue(const QString &key) const;
    QStringList allParameterValues(const QString &key) const;
    QByteArray requestBody() const;
    QString staticResourcePath() const;

    int hashCode() const;

    bool isLocalRequest() const;
    bool isStaticResourceRequest() const;
    bool isApiRequest() const;

    void setModule(QString);
    
signals:
    
public slots:
    
private:
    QString m_module;
};

#endif // STUBREQUEST_H
