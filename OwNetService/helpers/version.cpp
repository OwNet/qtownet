#include "version.h"

Version::Version(std::string const& versionStr, QObject *parent) :
    QObject(parent)
{
    // To Make processing easier in VersionDigit prepend a '.'
    std::stringstream   versionStream(std::string(".") + versionStr);

    // Copy all parts of the version number into the version Info vector.
    std::copy(  std::istream_iterator<VersionDigit>(versionStream),
                std::istream_iterator<VersionDigit>(),
                std::back_inserter(versionInfo)
             );
}

bool Version::operator<(Version const& rhs) const
{
    return std::lexicographical_compare(versionInfo.begin(), versionInfo.end(), rhs.versionInfo.begin(), rhs.versionInfo.end());
}

std::istream& operator>>(std::istream& str, Version::VersionDigit& digit)
{
    str.get();
    str >> digit.value;
    return str;
}
