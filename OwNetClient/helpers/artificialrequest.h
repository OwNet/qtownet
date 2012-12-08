#ifndef ARTIFICIALREQUEST_H
#define ARTIFICIALREQUEST_H

#include <QObject>
#include <QString>
#include <QMap>

#include "irequest.h"

class ArtificialRequest : public QObject, public IRequest
{
    Q_OBJECT
public:
    explicit ArtificialRequest(IRequest::RequestType requestType, const QString &service, const QString &url = QString(), QObject *parent = 0);
    explicit ArtificialRequest(IRequest::RequestType requestType, const QString &service, const int id, QObject *parent = 0);

    QVariant postBodyFromJson(bool *ok = NULL) const { return m_postBody; }

    IRequest::RequestType requestType() const { return m_requestType; }
    QString relativeUrl() const;

    QString service() const { return m_service; }
    QString subDomain() const { return m_submodule; }

    bool hasParameter(const QString& key) const { return m_parameters.contains(key); }

    QString parameterValue(const QString &key) const { return m_parameters.value(key); }
    void setParamater(const QString &key, const QString &value) { m_parameters.insert(key, value); }
    void setPostBody(const QVariantMap &body) { m_postBody = body; }

    IResponse* response();
    IResponse* response(const QVariant body, IResponse::Status status = IResponse::OK);
    IResponse* response(IResponse::Status status);

private:
    QString m_url;
    QString m_service;
    QString m_submodule;
    RequestType m_requestType;
    QMap<QString, QString> m_parameters;
    QVariantMap m_postBody;
};

#endif // ARTIFICIALREQUEST_H
