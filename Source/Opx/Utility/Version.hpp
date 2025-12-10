#pragma once

#include "../DataTypes.hpp"

OPX_NAMESPACE_BEGIN

class Version {
public:
    Int32 major;
    Int32 minor;
    Int32 patch;

    OPX_CONSTEXPR Version() : major(0), minor(0), patch(0) {}
    OPX_CONSTEXPR Version(Int32 major, Int32 minor, Int32 patch)
        : major(major), minor(minor), patch(patch) {}
    OPX_CONSTEXPR Version(const Version&) = default;
    OPX_CONSTEXPR Version(Version&&) = default;
    Version& operator=(const Version&) = default;
    Version& operator=(Version&&) = default;
};

OPX_CONSTEXPR Bool operator==(const Version& a, const Version& b) {
    return a.major == b.major && a.minor == b.minor && a.patch == b.patch;
}

OPX_CONSTEXPR Bool operator!=(const Version& a, const Version& b) {
    return a.major != b.major || a.minor != b.minor || a.patch != b.patch;
}

OPX_CONSTEXPR Bool operator<(const Version& a, const Version& b) {
    if (a.major != b.major) {
        return a.major < b.major;
    }
    if (a.minor != b.minor) {
        return a.minor < b.minor;
    }
    return a.patch < b.patch;
}

OPX_CONSTEXPR Bool operator>(const Version& a, const Version& b) {
    return b < a;
}

OPX_CONSTEXPR Bool operator<=(const Version& a, const Version& b) {
    return !(b < a);
}

OPX_CONSTEXPR Bool operator>=(const Version& a, const Version& b) {
    return !(a < b);
}

OPX_NAMESPACE_END