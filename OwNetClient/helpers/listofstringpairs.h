#ifndef LISTOFSTRINGPAIRS_H
#define LISTOFSTRINGPAIRS_H

#include <QList>
#include <QPair>
#include <QString>

class ListOfStringPairs : public QList< QPair<QString, QString> >
{
public:
    ListOfStringPairs();

    void insert(const QString &, const QString &);
};

#endif // LISTOFSTRINGPAIRS_H
