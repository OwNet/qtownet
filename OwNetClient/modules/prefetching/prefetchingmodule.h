#ifndef PREFETCHINGMODULE_H
#define PREFETCHINGMODULE_H

#include <imodule.h>
#include <QByteArray>
#include "ibus.h"
#include <QMap>
#include <QString>
#include "loggedpage.h"
#include <QRegExp>
#include "messagehelper.h"

class PrefetchJob;

class PrefetchingModule : public IModule
{
public:
    explicit PrefetchingModule(QObject *parent = NULL);

   QByteArray* processRequest(IBus *bus, ProxyRequest *req);



private:
  // int m_pageCounter;
   int m_pageCounter;


   PrefetchJob* m_prefetchJob;

   //int registerPage(QString &url);
};

#endif // PREFETCHINGMODULE_H
