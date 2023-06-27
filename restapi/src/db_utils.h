#pragma once

#include "User.h"
#include "Error.h"

#include <variant>

namespace mkm
{
std::variant<User, ErrorCode> get_user_details(const std::string& username);

bool is_password_valid(const std::string& input_password, const std::string& stored_password_hash);
}   // namespace mkm