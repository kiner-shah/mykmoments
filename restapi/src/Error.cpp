#include "Error.h"

namespace mkm
{
std::string error_str(const ErrorCode e)
{
    switch(e)
    {
        case ErrorCode::OK: return "OK";
        case ErrorCode::USER_NOT_FOUND: return "User not found";
        case ErrorCode::INTERNAL_ERROR: return "Some internal error occured";
        case ErrorCode::AUTHENTICATION_ERROR: return "Invalid credentials provided";
        default: return "UNKNOWN ERROR";
    }
}
}   // namespace mkm