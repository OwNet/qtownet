#ifndef IMODULE_H
#define IMODULE_H

#include <QObject>
#include <QString>
#include "ibus.h"


/**
 * @brief The IModule class Interface for Modules (Abstract class)
 */
class IModule : public QObject
{
    Q_OBJECT
public:
    explicit IModule(QObject *parent = 0);
    const QString &url() { return m_url;}

    /**
     * @brief processRequest Virtual function for Processing request from proxy/
     *        every module derived from IModule has to implement this function in it's own way
     * @param bus
     * @param req
     * @return  response Bytes
     */
    virtual QByteArray* processRequest(IBus *bus, ProxyRequest *req) = 0;

protected:
    void setUrl(QString url) { m_url = url;}
    
signals:
    
public slots:

private:
    QString m_url;
    
};

#endif // IMODULE_H
