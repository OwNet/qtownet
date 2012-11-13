#ifndef DATABASEINITIALIZER_H
#define DATABASEINITIALIZER_H

#include <QObject>

class DatabaseInitializer
{
public:
    DatabaseInitializer();

    void init();

private:
    void createUserTable();


};

#endif // DATABASEINITIALIZER_H
