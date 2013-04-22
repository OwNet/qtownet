#include "ownetcloudserver.h"

#include "jsondocument/jsondocument.h"

#include <QDebug>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QVariantMap>
#include <QNetworkReply>
#include <QFileInfo>

const QString OwNetCloudServer::crashReportServiceURL = "http://yatta.fiit.stuba.sk/OwNetRestService/Feedback/Send";
const QString OwNetCloudServer::updateServiceURL = "http://yatta.fiit.stuba.sk/OwNetUpdate/";

OwNetCloudServer::OwNetCloudServer(QObject *parent) :
    QObject(parent)
{
}

void OwNetCloudServer::sendCrashReport(QString report)
{
    QVariantMap data;
    data.insert("feedback", "crash_report");
    data.insert("output", report);

    QByteArray postData;
    postData.append(JsonDocument::fromVariant(data).toJson());

    QNetworkRequest *request = new QNetworkRequest(QUrl(crashReportServiceURL));
    request->setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkAccessManager *nam = new QNetworkAccessManager(this);
    nam->post(*request, postData);
}

void OwNetCloudServer::checkUpdateMetadata()
{
    QNetworkRequest *request = new QNetworkRequest(QUrl(QString(updateServiceURL).append("metadata.json")));

    QNetworkAccessManager *nam = new QNetworkAccessManager(this);
    connect(nam, SIGNAL(finished(QNetworkReply*)), this, SLOT(updateMetadataReceived(QNetworkReply*)));
    nam->get(*request);
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

void OwNetCloudServer::downloadUpdatePackage(QString packageName)
{
    QString url = QString(updateServiceURL).append(packageName);
    QNetworkRequest *request = new QNetworkRequest(QUrl(url));

    QNetworkAccessManager *nam = new QNetworkAccessManager(this);
    connect(nam, SIGNAL(finished(QNetworkReply*)), this, SLOT(packageDataReceived(QNetworkReply*)));
    nam->get(*request);
}

void OwNetCloudServer::packageDataReceived(QNetworkReply *reply)
{
    QFileInfo fileInfo(reply->request().url().path());
    QByteArray data = reply->readAll();

    emit packageDataReceived(fileInfo.fileName(), data);
}
