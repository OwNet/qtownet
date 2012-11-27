#ifndef PREFETCHJOB_H
#define PREFETCHJOB_H


#include "job.h"

#include <QMutex>
#include <QMap>
#include <QtWebKit>
class LoggedPage;



class PrefetchJob : public Job
{
public:
    PrefetchJob(QObject *parent = NULL);
    void registerLink(int id, QString url);
    void registerPage(int id, QString url);
    void removePage(int id);
    void removePage(QString url);
protected:
    void execute();

private:
    QMap<int, LoggedPage*> m_map;
    void prefetch();
    QWebView m_webView;
    QMutex m_activeMutex;
    QMutex m_mapMutex;
    int m_last;
};

#endif // PREFETCHJOB_H
