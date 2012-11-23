#include "variantmap.h"

#include "qjson/serializer.h"
#include "qjson/parser.h"

#include <QDebug>

VariantMap::VariantMap()
{
}

VariantMap::VariantMap(const QVariantMap &other)
    : QVariantMap(other)
{
}

QString VariantMap::toJsonString() const
{
    QJson::Serializer serializer;
    return QString(serializer.serialize(QVariant(*this)));
}

void VariantMap::insertCaseInsensitive(const QString &key, const QVariant &value)
{
    QString lowerKey = key.toLower();
    foreach (QString i, keys()) {
        if (i.toLower() == lowerKey) {
            insert(i, value);
            return;
        }
    }
    insert(key, value);
}
