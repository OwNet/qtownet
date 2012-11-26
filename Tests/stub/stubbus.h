#ifndef STUBBUS_H
#define STUBBUS_H

#include <QObject>

#include "../OwNetClient/modules/ibus.h"

class StubBus : public QObject, public IBus
{
    Q_OBJECT
public:
    explicit StubBus(QObject *parent = 0);
    
    QByteArray* callModule(IRequest *req);

    void setHttpStatus(int code, const QString &description);

    ISession *session();

    IDatabaseUpdate *databaseUpdate();

    QSettings *settings();
signals:
    
public slots:
    
};

#endif // STUBBUS_H
