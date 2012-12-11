#ifndef PORTALHELPER_H
#define PORTALHELPER_H

#include <QObject>

class PortalHelper : public QObject
{
    Q_OBJECT
public:
    explicit PortalHelper(QObject *parent = 0);

    static void addSalt( QString *password, QString *salt);
    

    
};

#endif // PORTALHELPER_H
