#include "discord_rpc_helper.hpp"
#include "discord/rich_presence.hpp"
#include "ida_helper.hpp"

#include <string>

extern idarpc::discord::RichPresence* rpc;

namespace idarpc::discord_rpc_helper {

    void update_presence(const discord_helper_spec& spec) {

        if (!rpc)
            return;

        DiscordRichPresence presence{};
        presence.state = spec.state.c_str();
        presence.details = spec.details.empty() ? idahelper::get_filename() : spec.details.c_str();
        presence.largeImageKey = spec.large_image_key.c_str();
        presence.largeImageText = spec.large_image_text.c_str();
        presence.smallImageKey = spec.small_image_key.c_str();
        presence.smallImageText = spec.small_image_text.c_str();

        rpc->update_presence(presence);
    }
}