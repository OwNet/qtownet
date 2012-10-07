#ifndef PROXYINPUTOBJECT_H
#define PROXYINPUTOBJECT_H

#include <QObject>

class ProxyInputObject : public QObject
{
    Q_OBJECT

public:
    ProxyInputObject(QObject *parent = 0);
    
signals:
    void readyRead();
    void finished();
    
public slots:
    
};

#endif // PROXYINPUTOBJECT_H
