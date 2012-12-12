#ifndef VARIANTMAP_H
#define VARIANTMAP_H

#include <QVariantMap>

class VariantMap : public QVariantMap
{
public:
    VariantMap();
    VariantMap(const QVariantMap &other);

    VariantMap* set(const QString &key, const QVariant &value);

    QString toJsonString() const;
    void insertCaseInsensitive(const QString &key, const QVariant &value);
};

#endif // VARIANTMAP_H
