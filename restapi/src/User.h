#pragma once

#include <string>

namespace mkm
{
struct User
{
    std::string username;
    std::string password_hash;
    std::string full_name;
    std::string birth_date;
    std::string email_id;
    std::string account_creation_date;
    // TODO: last login
};

}   // namespace mkm