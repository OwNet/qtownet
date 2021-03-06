#include "variantmap.h"
#include "jsondocument.h"

#include <QDebug>

VariantMap::VariantMap()
{
}

VariantMap::VariantMap(const QVariantMap &other)
    : QVariantMap(other)
{
}

VariantMap *VariantMap::set(const QString &key, const QVariant &value)
{
    this->insert(key,value);
    return this;
}

QString VariantMap::toJsonString() const
{    
    return QString( JsonDocument::fromVariantMap(*this).toJson() );
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
