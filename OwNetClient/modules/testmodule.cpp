#include "testmodule.h"

TestModule::TestModule(QObject *parent) :
    IModule(parent)
{
    setUrl("test");
}

/**
 * @brief TestModule::processRequest Processing of request
 * @param bus
 * @param req
 * @return byte array
 */
QByteArray* TestModule::processRequest(IBus *bus, ProxyRequest *req)
{
    // own body of processing request

    // if you need to call another module call bus.callModule( ProxyRequest req)

    return new QByteArray();

}
