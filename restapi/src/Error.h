#pragma once

#include <cstdint>
#include <string>
#include <map>

namespace mkm
{

enum class ErrorCode : uint8_t
{
    OK = 0,
    USER_NOT_FOUND,
    INTERNAL_ERROR,
    AUTHENTICATION_ERROR
};

std::string error_str(const ErrorCode e);
}   // namespace mkm