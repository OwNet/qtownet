#ifndef APPLICATIONENVIRONMENT_H
#define APPLICATIONENVIRONMENT_H

#include <QObject>
#include <QProcessEnvironment>

class ApplicationEnvironment : public QObject
{
    Q_OBJECT
public:
    explicit ApplicationEnvironment(QObject *parent = 0);
    
    QString value( const QString & name, const QString & defaultValue = QString() ) const;
    bool contains ( const QString & name ) const;

    void init();
};

#endif // APPLICATIONENVIRONMENT_H
