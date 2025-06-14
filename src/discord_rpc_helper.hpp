#pragma once

#include <string>

struct discord_helper_spec
{
    std::string state;
    std::string details;
    std::string large_image_key;
    std::string large_image_text;
    std::string small_image_key;
    std::string small_image_text;
};

namespace idarpc::discord_rpc_helper
{
    void update_presence(const discord_helper_spec &spec);
}
