#include "databasemodule.h"
#include "messagehelper.h"

DatabaseModule::DatabaseModule(QObject *parent) :
    IModule(parent)
{
    setUrl("db");
}

/**
 * @brief DatabaseModule::processRequest Processing of request
 * @param bus
 * @param req
 * @return byte array
 */
QByteArray* DatabaseModule::processRequest(IBus *bus, ProxyRequest *req)
{

    MessageHelper::debug("db module");

    return new QByteArray("hello db module");

}

