#ifndef JSONDOCUMENT_H
#define JSONDOCUMENT_H

#include <QJsonDocument>
#include "qjson_p.h"


class JsonDocument : public QJsonDocument
{
public:    
    explicit JsonDocument();
    explicit JsonDocument(const QJsonObject &object);
    explicit JsonDocument(const QJsonArray &array);

    JsonDocument(const QJsonDocument &other);
    JsonDocument &operator =(const QJsonDocument &other);

    QByteArray toJson() const;
    QByteArray toJson(bool compact) const;
    
    static JsonDocument fromJson(const QByteArray & json, bool *ok);
    static JsonDocument fromJson(const QByteArray &json, QJsonParseError *error = 0);

    static JsonDocument fromVariantList(const QVariantList &list);
    static JsonDocument fromVariantMap(const QVariantMap &map);

private:
    friend class QJsonValue;
    friend class QJsonPrivate::Data;
    friend class QJsonPrivate::Parser;
    friend Q_CORE_EXPORT QDebug operator<<(QDebug, const QJsonDocument &);

    QJsonPrivate::Data *d;

};

#endif // JSONDOCUMENT_H
