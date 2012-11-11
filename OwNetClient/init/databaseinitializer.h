#ifndef DATABASEINITIALIZER_H
#define DATABASEINITIALIZER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QFile>
#include <QSqlQuery>

class DatabaseInitializer
{
public:
    DatabaseInitializer();

    void init();
private:

    QSqlDatabase db;
    void createUsersTable();
    void dropUsersTable();


};

#endif // DATABASEINITIALIZER_H
