#pragma once

#include <string>

namespace idarpc::discord_rpc_helper
{
    void render_presence();

    void tick();

    void mark_activity();

    void reload_presence();

    void reconnect();

    bool is_connected();

    std::string status_text();
}
