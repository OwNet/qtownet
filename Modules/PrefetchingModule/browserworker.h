#ifndef BROWSERWORKER_H
#define BROWSERWORKER_H

#include <QObject>
#include <QUrl>
#include <QWebView>
class QTimer;
class IProxyConnection;


class BrowserWorker : public QObject
{
    Q_OBJECT
public:
    explicit BrowserWorker(QString &link, QObject *parent = 0);
private:
    QUrl m_link;
    QWebView m_webView;
    QTimer *m_timer;
    void resetBrowser();
signals:
    void workCompleted();

private slots:
    void finished(bool);
    void timedOut();
public slots:
    void doWork();
};

#endif // BROWSERWORKER_H




