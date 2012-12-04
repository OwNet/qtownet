#ifndef ARTIFICIALBUS_H
#define ARTIFICIALBUS_H

#include <QObject>

#include "ibus.h"

class ArtificialBus : public QObject, public IBus
{
    Q_OBJECT
public:
    explicit ArtificialBus(QObject *parent = 0);

    QString contentType() const { return m_contentType; }
    void setContentType(const QString &value) { m_contentType = value; }

    void setHttpStatus(int, const QString &) {}

private:
    QString m_contentType;
};

#endif // ARTIFICIALBUS_H
