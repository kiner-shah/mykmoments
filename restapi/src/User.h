#pragma once

#include <string>

namespace mkm
{
struct User
{
    std::string username;
    std::string password_hash;
    std::string user_full_name;
    // TODO: add birthdate, age, last login
};

}   // namespace mkm