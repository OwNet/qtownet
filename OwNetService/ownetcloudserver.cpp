#include "ownetcloudserver.h"

#include "jsondocument/jsondocument.h"

#include <QDebug>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QVariantMap>

OwNetCloudServer::OwNetCloudServer(QObject *parent) :
    QObject(parent)
{
    m_nam = new QNetworkAccessManager(this);
}

void OwNetCloudServer::sendCrashReport(QString report)
{
    QVariantMap data;
    data.insert("feedback", "crash_report");
    data.insert("output", report);

    QString url = "http://localhost:10000";
    QByteArray postData;
    postData.append(JsonDocument::fromVariant(data).toJson());

    QNetworkRequest *request = new QNetworkRequest(QUrl(url));
    request->setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    m_nam->post(*request, postData);
}
