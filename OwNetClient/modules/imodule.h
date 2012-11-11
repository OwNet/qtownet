#ifndef IMODULE_H
#define IMODULE_H

#include <QObject>
#include <QString>
#include "ibus.h"
#include <QByteArray>


class IModule;

/**
 * @brief The IModule class Interface for Modules (Abstract class)
 */
class IModule : public QObject
{

    Q_OBJECT
public:

    enum Action {
        INDEX,
        SHOW,
        CREATE,
        EDIT,
        DELETE,
        UNKNOWN
    };

    explicit IModule(QObject *parent = 0);
    const QString &url() { return m_url;}

    /**
     * @brief processRequest Virtual function for Processing request from proxy/
     *        every module derived from IModule has to implement this function in it's own way
     * @param bus
     * @param req
     * @return  response Bytes
     */
    QByteArray* processRequest(IBus *bus, ProxyRequest *req);

protected:
    void setUrl(QString url) { m_url = url;}

signals:
    
public slots:

private:
    QString m_url;



    virtual QByteArray* index(IBus *bus,  ProxyRequest *req){return new QByteArray();}

    // create element
    virtual QByteArray* create(IBus *bus,  ProxyRequest *req){return new QByteArray();}

    // show element
    virtual QByteArray* show( IBus *bus,  ProxyRequest *req){return new QByteArray();}

    //delete element
    virtual QByteArray* del( IBus *bus,  ProxyRequest *req){return new QByteArray();}

    //edit element
    virtual QByteArray* edit( IBus *bus,  ProxyRequest *req){return new QByteArray();}
    
};

#endif // IMODULE_H
