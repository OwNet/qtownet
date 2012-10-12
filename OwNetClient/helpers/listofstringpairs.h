#ifndef LISTOFSTRINGPAIRS_H
#define LISTOFSTRINGPAIRS_H

#include <QList>
#include <QPair>
#include <QString>

class ListOfStringPairs : public QList< QPair<QString, QString> >
{
public:
    ListOfStringPairs();
};

#endif // LISTOFSTRINGPAIRS_H
