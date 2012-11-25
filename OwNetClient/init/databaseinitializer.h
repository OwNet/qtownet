#ifndef DATABASEINITIALIZER_H
#define DATABASEINITIALIZER_H

#include <QObject>

class DatabaseInitializer
{
public:
    DatabaseInitializer();

    void init();

private:
    void openDatabase();
    void runMigrations();
};

#endif // DATABASEINITIALIZER_H