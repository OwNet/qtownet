#include "databasemodule.h"
#include "qjson/serializer.h"

#include <QBuffer>
#include <QVariantList>
#include <QtCore/QVariant>
#include <QSqlQuery>

#include "messagehelper.h"

DatabaseModule::DatabaseModule()
{
}

/**
 * @brief DatabaseModule::processRequest Processing of request
 * @param bus
 * @param req
 * @return byte array
 */

/*QByteArray* DatabaseModule::processRequest(IBus *bus, ProxyRequest *req)
{
    QVariantMap response;
    QJson::Serializer serializer;

    //
    Not implemented

    MessageHelper::debug("Querying...");

    QSqlQuery q;
    q.exec("create table pages (id INTEGER PRIMARY KEY,url TEXT);");
    q.exec("insert into pages (url) values (\"http://www.qt.com\");");
    q.exec("SELECT * FROM pages;");
    while (q.next()) {
        MessageHelper::debug(QObject::tr("%1 %2").arg(q.value(0).toString()).arg(q.value(1).toString()));
    }


    response.insert("ok", "<data>");

    return new QByteArray(serializer.serialize(response));
}*/

