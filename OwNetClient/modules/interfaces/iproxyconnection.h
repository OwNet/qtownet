#ifndef IPROXYCONNECTION_H
#define IPROXYCONNECTION_H

#include <QString>

#include "irequest.h"

class ISession;
class IDatabaseUpdate;
class QSettings;
class IDatabaseSettings;
class IDatabaseSelectQuery;

class IProxyConnection
{
public:
    virtual ISession *session(QObject *parent = 0) = 0;
    virtual IDatabaseUpdate *databaseUpdate(QObject *parent = 0) = 0;
    virtual IDatabaseSelectQuery *databaseSelect(const QString &table, QObject *parent = 0) = 0;
    virtual QSettings *settings(QObject *parent = 0) = 0;
    virtual IDatabaseSettings *databaseSettings(QObject *parent = 0) = 0;
    virtual IRequest *createRequest(IRequest::RequestType requestType, const QString &module, const QString &action = QString(), int id = -1, QObject *parent = 0) = 0;
    virtual QVariant fromJson(const QByteArray &content, bool *ok = NULL) const = 0;
    virtual QByteArray toJson(const QVariant &content) const = 0;

    /**
     * @brief CallModule to enable calls between modules
     * @param req
     * @return response byte array
     */
    virtual QVariant *callModule(IRequest *req) = 0;
};

#endif // IPROXYCONNECTION_H
