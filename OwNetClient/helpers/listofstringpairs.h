#ifndef LISTOFSTRINGPAIRS_H
#define LISTOFSTRINGPAIRS_H

#include <QList>
#include <QPair>
#include <QString>
#include <QVariantMap>

class ListOfStringPairs : public QList< QPair<QString, QString> >
{
public:
    ListOfStringPairs();

    void insert(const QString &, const QString &);
    void parse(const QVariantMap &variantMap);
    QString toString();
};

#endif // LISTOFSTRINGPAIRS_H
