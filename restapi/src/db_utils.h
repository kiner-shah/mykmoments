#pragma once

#include "User.h"
#include "Error.h"

#include <variant>

namespace mkm
{
std::variant<User, ErrorCode> get_user_details(const std::string& username);

bool is_password_valid(const std::string& input_password, const std::string& stored_password_hash);

bool create_new_account(const std::string& full_name, const std::string& birth_date, const std::string& email_id, const std::string& username, const std::string& password);
}   // namespace mkm