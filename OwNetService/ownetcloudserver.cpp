#include "ownetcloudserver.h"

#include <QDebug>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkAccessManager>

OwNetCloudServer::OwNetCloudServer(QObject *parent) :
    QObject(parent)
{
}

void OwNetCloudServer::sendCrashReport(QString report)
{
    QNetworkAccessManager *nam = new QNetworkAccessManager(this);

    QString url = "http://localhost:10000";
    QByteArray postData;
    postData.append(report);

    QNetworkRequest *request = new QNetworkRequest(QUrl(url));
    request->setHeader(QNetworkRequest::ContentTypeHeader, "text/plain");

    nam->post(*request, postData);
}
