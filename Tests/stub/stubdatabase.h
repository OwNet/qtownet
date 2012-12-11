#ifndef STUBDATABASE
#define STUBDATABASE

class StubDatabase
{
public:    
    static void init();
    static void close();

private:
    static void runMigrations();
};

#endif // STUBDATABASE
