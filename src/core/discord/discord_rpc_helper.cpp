#include "discord_rpc_helper.hpp"
#include "discord/rich_presence.hpp"
#include "ida_helper.hpp"

#include <pro.h>
#include <kernwin.hpp>
#include <idp.hpp>
#include <loader.hpp>
#include <name.hpp>
#include <bytes.hpp>

extern idarpc::discord::RichPresence *rpc;

static time_t start_time = time(nullptr);

std::string get_ida_version_string()
{
    char version[32] = {0};
    get_kernel_version(version, sizeof(version));
    return std::string(version);
}

namespace idarpc::discord_rpc_helper
{
    void update_presence(const discord_helper_spec &spec)
    {
        if (!rpc)
            return;

        DiscordRichPresence presence{};
        presence.startTimestamp = start_time;

        if (idarpc::idahelper::is_ida_home_version())
        {
            presence.largeImageKey = "ida_home";
            std::string homeText = "IDA HOME " + get_ida_version_string();
            presence.largeImageText = homeText.c_str();
        }
        else
        {
            presence.largeImageKey = "ida_pro";
            std::string proText = "IDA PRO " + get_ida_version_string();
            presence.largeImageText = proText.c_str();
        }

        presence.state = idahelper::get_filename();
        presence.details = spec.details.c_str();
        presence.smallImageKey = spec.small_image_key.c_str();
        presence.smallImageText = spec.small_image_text.c_str();

        rpc->update_presence(presence);
    }

}