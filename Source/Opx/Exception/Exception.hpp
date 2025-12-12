#pragma once

#include "../DataTypes.hpp"
#include <exception>

OPX_NAMESPACE_BEGIN

class Exception {
public:
    Exception() noexcept {}
    Exception(const Exception&) = default;
    Exception(Exception&&) = default;
    Exception& operator=(const Exception&) = default;
    Exception& operator=(Exception&&) = default;
    virtual ~Exception() noexcept = default;

    virtual const Char* Message() const noexcept { return "Opx::Exception"; }
    virtual const Char* what() const noexcept { return Message(); }
};

OPX_NAMESPACE_END