#include "ownetcloudserver.h"

#include "jsondocument/jsondocument.h"

#include <QDebug>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QVariantMap>
#include <QNetworkReply>

OwNetCloudServer::OwNetCloudServer(QObject *parent) :
    QObject(parent)
{
    m_nam = new QNetworkAccessManager(this);

    connect(m_nam, SIGNAL(finished(QNetworkReply*)), this, SLOT(updateMetadataReceived(QNetworkReply*)));
}

void OwNetCloudServer::sendCrashReport(QString report)
{
    QVariantMap data;
    data.insert("feedback", "crash_report");
    data.insert("output", report);

    QString url = "http://localhost:3000/feedback";
    QByteArray postData;
    postData.append(JsonDocument::fromVariant(data).toJson());

    QNetworkRequest *request = new QNetworkRequest(QUrl(url));
    request->setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    m_nam->post(*request, postData);
}

void OwNetCloudServer::checkUpdateMetadata()
{
    QString url = "http://localhost:3000/update/metadata.json";
    QNetworkRequest *request = new QNetworkRequest(QUrl(url));
    m_nam->get(*request);
}

void OwNetCloudServer::updateMetadataReceived(QNetworkReply *reply)
{
    QByteArray json = reply->readAll();
    bool ok = false;
    QVariantMap metadata = JsonDocument::fromJson(json, &ok).toVariant().toMap();
    if (ok) {
        emit updateMetadataReceived(metadata);
    }
}
