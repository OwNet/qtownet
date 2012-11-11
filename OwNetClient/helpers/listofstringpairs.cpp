#include "listofstringpairs.h"

#include "qjson/serializer.h"

ListOfStringPairs::ListOfStringPairs()
    : QList< QPair<QString, QString> >()
{
}

void ListOfStringPairs::insert(const QString &key, const QString &value)
{
    QPair<QString, QString> pair(key, value);
    append(pair);
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
    for (int i = 0; i < count(); ++i) {
        map.insert(at(i).first, at(i).second);
    }

    QJson::Serializer serializer;
    return QString(serializer.serialize(map));
}
