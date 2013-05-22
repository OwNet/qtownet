#ifndef IWEBDOWNLOAD_H
#define IWEBDOWNLOAD_H

#include <QObject>

class IWebDownload
{
public:
    virtual void downloadFailed() = 0;
    virtual void downloadFinished(qint64 size) = 0;
};

#endif // IWEBDOWNLOAD_H
