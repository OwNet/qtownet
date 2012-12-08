#include "listofstringpairs.h"
#include "jsondocument.h"

ListOfStringPairs::ListOfStringPairs()
    : QList< QPair<QString, QString> >()
{
}

void ListOfStringPairs::insert(const QString &key, const QString &value)
{
    QPair<QString, QString> pair(key, value);
    append(pair);
}

void ListOfStringPairs::insertOrReplace(const QString &key, const QString &value)
{
    for (int i = count() - 1; i >= 0; --i)
        if (QString::compare(at(i).first, key, Qt::CaseInsensitive) == 0)
            removeAt(i);
    insert(key, value);
}

void ListOfStringPairs::parse(const QVariantMap &variantMap)
{
    foreach (QString key, variantMap.keys()) {
        insert(key, variantMap.value(key).toString());
    }
}

QString ListOfStringPairs::toString()
{
    QVariantMap map;
    for (int i = 0; i < count(); ++i)
        map.insert(at(i).first, at(i).second);

    return QString(JsonDocument::fromVariantMap(map).toJson());
}

QString ListOfStringPairs::valueForKey(const QString &key) const
{
    for (int i = 0; i < count(); i++){
        if (QString::compare(at(i).first, key, Qt::CaseInsensitive) == 0)
            return at(i).second;
    }
    return "";
}
