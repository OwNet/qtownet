#ifndef IBUS_H
#define IBUS_H

#include <QByteArray>
#include "proxyrequest.h"

class IBus
{

public:
    explicit IBus();
    
    /**
     * @brief CallModule to enable calls between modules
     * @param req
     * @return response byte array
     */
    virtual QByteArray* callModule(ProxyRequest *req) = 0;

    
};

#endif // IBUS_H
