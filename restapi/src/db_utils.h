#pragma once

#include "User.h"
#include "Error.h"
#include "Moment.h"

#include <variant>
#include <optional>

namespace mkm
{
std::variant<User, ErrorCode> get_user_details(const std::string& username);

bool is_password_valid(const std::string& input_password, const std::string& stored_password_hash);

bool create_new_account(const User& user_details, const std::string& password);

bool add_new_moment(const Moment& moment);

bool update_moment(const Moment& moment);

bool delete_moment(const std::string& username, uint64_t moment_id);

uint64_t get_moment_count(const std::string& username);

std::variant< std::vector<Moment>, ErrorCode > get_moments_list(const std::string& username, uint32_t page_size, uint64_t current_page, std::optional<std::string> sort_by, std::optional<std::string> search);

std::variant<Moment, ErrorCode> get_moment_details(const std::string& username, uint64_t id);
}   // namespace mkm