#ifndef STUBDATABASE
#define STUBDATABASE

#include <QString>

class StubDatabase
{
public:    
    static void init();
    static void close();

    static void dumpTable(const QString &tableName);

private:
    static void runMigrations();
};

#endif // STUBDATABASE
