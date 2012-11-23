#include "applicationenvironment.h"

ApplicationEnvironment::ApplicationEnvironment(QObject *parent) :
    QObject(parent)
{
}

 QString ApplicationEnvironment::value ( const QString & name, const QString & defaultValue) const
 {
    return QProcessEnvironment::systemEnvironment().value(name, defaultValue);
 }

 bool ApplicationEnvironment::contains ( const QString & name ) const
 {
     return QProcessEnvironment::systemEnvironment().contains(name);
 }
