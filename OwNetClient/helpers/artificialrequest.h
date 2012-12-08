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
    explicit ArtificialRequest(IRequest::RequestType requestType, const QString &module, const QString &action = QString(), int id = -1, QObject *parent = 0);

    QVariant postBodyFromJson(bool *ok = NULL) const { return m_postBody; }
    void setPostBody(const QVariant &body) { m_postBody = body; }

    IRequest::RequestType requestType() const { return m_requestType; }

    QString action() const { return m_action; }
    QString module() const { return m_module; }
    QString subDomain() const { return m_submodule; }
    int id() const { return m_id; }
    bool hasParameter(const QString& key) const { return m_parameters.contains(key); }
    QString parameterValue(const QString &key) const { return m_parameters.value(key); }
    void setParamater(const QString &key, const QString &value) { m_parameters.insert(key, value); }

private:
    int m_id;
    QString m_action;
    QString m_module;
    QString m_submodule;
    RequestType m_requestType;
    QMap<QString, QString> m_parameters;
    QVariant m_postBody;
};

#endif // ARTIFICIALREQUEST_H
