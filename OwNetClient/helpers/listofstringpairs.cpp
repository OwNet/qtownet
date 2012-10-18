#include "listofstringpairs.h"

ListOfStringPairs::ListOfStringPairs()
    : QList< QPair<QString, QString> >()
{
}

void ListOfStringPairs::insert(const QString &key, const QString &value)
{
    QPair<QString, QString> pair(key, value);
    this->append(pair);
}
