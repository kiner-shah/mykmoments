#pragma once

#include <string>
#include <vector>

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
    std::string image_content;
    std::string image_caption;
    std::vector<std::string> feelings;
};

}   // namespace mkm