#ifndef VERSION_H
#define VERSION_H

#include <QObject>

#include <iostream>
#include <sstream>
#include <vector>
#include <iterator>

class Version : public QObject
{
    Q_OBJECT

    // An internal utility structure just used to make the std::copy in the constructor easy to write.
    struct VersionDigit
    {
        int value;
        operator int() const {return value;}
    };
    friend std::istream& operator>>(std::istream& str, Version::VersionDigit& digit);
    public:
        Version(std::string const&, QObject *parent = 0);

        bool operator<(Version const&) const;

    private:
        std::vector<int>    versionInfo;
};

#endif // VERSION_H
