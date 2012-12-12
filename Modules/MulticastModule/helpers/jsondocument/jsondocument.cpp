#include "jsondocument.h"
#include "qjson_p.h"
#include "qjsonwriter_p.h"

JsonDocument::JsonDocument() :
    QJsonDocument()
{
}

JsonDocument::JsonDocument(const QJsonDocument &other) :
    QJsonDocument(other)
{

}

JsonDocument::JsonDocument(const QJsonObject &object) :
    QJsonDocument(object)
{

}

JsonDocument::JsonDocument(const QJsonArray &array) :
    QJsonDocument(array)
{

}

QByteArray JsonDocument::toJson(bool compact) const
{
    if (isEmpty())
        return QByteArray();

    int size;
    const char* raw = rawData(&size);
    QJsonPrivate::Data d(const_cast<char*>(raw),size,false);

    QByteArray json;

    if (d.header->root()->isArray())
        QJsonPrivate::Writer::arrayToJson(static_cast<QJsonPrivate::Array *>(d.header->root()), json, 0, compact);
    else
        QJsonPrivate::Writer::objectToJson(static_cast<QJsonPrivate::Object *>(d.header->root()), json, 0, compact);

    return json;
}

JsonDocument JsonDocument::fromJson(const QByteArray & json, bool *ok)
{
    QJsonParseError err;
    JsonDocument doc = JsonDocument::fromJson(json, &err);

    *ok = ( err.error == QJsonParseError::NoError);
    return doc;
}

JsonDocument JsonDocument::fromJson(const QByteArray &json, QJsonParseError *error)
{
    return QJsonDocument::fromJson(json, error);
}

JsonDocument JsonDocument::fromVariant(const QVariant &varaint)
{
    return JsonDocument( QJsonDocument::fromVariant(varaint) );
}

JsonDocument JsonDocument::fromVariantList(const QVariantList &list)
{
    QJsonArray array = QJsonArray::fromVariantList(list);
    return JsonDocument(array);
}

JsonDocument JsonDocument::fromVariantMap(const QVariantMap &map)
{
    QJsonObject object = QJsonObject::fromVariantMap(map);
    return JsonDocument(object);
}

