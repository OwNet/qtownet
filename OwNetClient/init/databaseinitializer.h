#ifndef DATABASEINITIALIZER_H
#define DATABASEINITIALIZER_H

#include <QObject>

class DatabaseInitializer : public QObject
{
    Q_OBJECT

public:
    DatabaseInitializer(QObject *parent = NULL);

    void init();

private:
    void openDatabase();
    void runMigrations();
    void createClientName();
    QString databaseName() const;
};

#endif // DATABASEINITIALIZER_H
