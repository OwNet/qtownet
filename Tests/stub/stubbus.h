#ifndef STUBBUS_H
#define STUBBUS_H

#include <QObject>

#include "../OwNetClient/modules/interfaces/ibus.h"

class StubBus : public QObject, public IBus
{
    Q_OBJECT
public:
    explicit StubBus(QObject *parent = 0);

    QVariant *callModule(IRequest *req);

    QString contentType() const;
    void setContentType(const QString &value);

    void setHttpStatus(int code, const QString &description);
signals:
    
public slots:
    
};

#endif // STUBBUS_H
