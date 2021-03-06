#include "browserworker.h"

#include <QNetworkProxy>
#include <QDateTime>
#include <QStringList>

#include <QWebView>
#include <QDebug>
#include <QTimer>
#include <QUrl>
#include "noalertwebpage.h"
BrowserWorker::BrowserWorker(QString &link, QObject *parent) :
    QObject(parent),
    m_link(QUrl(link))
{
    QNetworkProxy proxy(QNetworkProxy::HttpProxy,
                        QString("127.0.0.1"),
                        8081,
                        QString(""),
                        QString(""));
    m_webView.setPage(new NoAlertWebPage());

    m_webView.page()->networkAccessManager()->setProxy(proxy);
    connect(&m_webView, SIGNAL(loadFinished(bool)), this, SLOT(finished(bool)));

    m_timer = new QTimer(this);
    m_timer->setInterval(1 * 60 * 1000);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(timedOut()));
}

void BrowserWorker::doWork() {
    m_webView.load(m_link);
    m_timer->start();
}

void BrowserWorker::finished(bool) {
    QUrl url = m_webView.url();

    if (url.toString().remove("/") == m_link.toString().remove("/")) {
        resetBrowser();
    }
}

void BrowserWorker::resetBrowser() {
    disconnect(&m_webView, SIGNAL(loadFinished(bool)), this, SLOT(finished(bool)));
    m_webView.load(QUrl("about:blank"));
    emit workCompleted();
}

void BrowserWorker::timedOut() {
    resetBrowser();
}

