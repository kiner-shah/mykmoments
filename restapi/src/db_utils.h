#pragma once

#include "User.h"
#include "Error.h"
#include "Moment.h"

#include <variant>

namespace mkm
{
std::variant<User, ErrorCode> get_user_details(const std::string& username);

bool is_password_valid(const std::string& input_password, const std::string& stored_password_hash);

bool create_new_account(const User& user_details, const std::string& password);

bool add_new_moment(const Moment& moment);

uint64_t get_moment_count(const std::string& username);
}   // namespace mkm