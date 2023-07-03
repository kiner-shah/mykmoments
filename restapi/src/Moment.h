#pragma once

#include <string>
#include <vector>
#include <cstddef>

namespace mkm
{
struct Moment
{
    uint64_t id;
    std::string username;
    std::string title;
    std::string description;
    std::string date;
    std::string image_filename;
    std::vector<std::byte> image_content;
    std::string image_caption;
    std::string created_date;
    std::string last_modified_date;
    std::vector<std::string> feelings;
};

}   // namespace mkm