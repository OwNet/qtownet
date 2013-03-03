#ifndef OWNETINSTALLATION_H
#define OWNETINSTALLATION_H

#include <QObject>
#include <QString>

class OwNetInstallation : public QObject
{
    Q_OBJECT
public:
    explicit OwNetInstallation(QObject *parent = 0);
    QString currentVersion();
    
signals:
    
public slots:
    
private:
    QString m_currentVersion;
};

#endif // OWNETINSTALLATION_H
