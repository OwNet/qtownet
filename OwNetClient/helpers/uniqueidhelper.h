#ifndef UNIQUEIDHELPER_H
#define UNIQUEIDHELPER_H

#include <QObject>

class UniqueIdHelper : public QObject
{
    Q_OBJECT
public:
    explicit UniqueIdHelper(QObject *parent = 0);

    static QString generate();
};

#endif // UNIQUEIDHELPER_H
