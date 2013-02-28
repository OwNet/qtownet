#ifndef NOALERTWEBPAGE_H
#define NOALERTWEBPAGE_H

#include <QWebPage>

class NoAlertWebPage : public QWebPage
{
public:
    NoAlertWebPage() {}
protected:
    virtual void javaScriptAlert (QWebFrame*, const QString &) {}
};

#endif // NOALERTWEBPAGE_H
