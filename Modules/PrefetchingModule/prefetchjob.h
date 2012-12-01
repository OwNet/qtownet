#ifndef PREFETCHJOB_H
#define PREFETCHJOB_H


#include <QMutex>
#include <QMap>
#include <QWebView>
#include "ijobaction.h"
#include <QObject>

class LoggedPage;



class PrefetchJob : public QObject, public IJobAction
{
    Q_OBJECT
public:
    PrefetchJob();
    void registerLink(int id, QString url);
    void registerPage(int id, QString url);
    void removePage(int id);

    int interval() { return 1 * 60 * 1000; }
    void execute();


    void removePage(QString url);

private:

    QMap<int, LoggedPage*> m_map;
    void prefetch();
    QWebView m_webView;
    QMutex m_activeMutex;
    QMutex m_mapMutex;
    int m_last;
private slots:
      void resetBrowser(bool);
};

#endif // PREFETCHJOB_H
