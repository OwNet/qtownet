#ifndef IBUS_H
#define IBUS_H

#include <QByteArray>

class IRequest;

class IBus
{

public:
    /**
     * @brief CallModule to enable calls between modules
     * @param req
     * @return response byte array
     */
    virtual QByteArray* callModule(IRequest *req) = 0;

    virtual void setHttpStatus(int code, const QString &description) = 0;
};

#endif // IBUS_H
