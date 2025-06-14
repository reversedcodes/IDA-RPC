#pragma once

#include <string>
#include <discord_rpc.h>

namespace idarpc::discord {

    class RichPresence {
    public:
        explicit RichPresence(const char* app_id);
        ~RichPresence();

        void update_presence(DiscordRichPresence rpc);

        void clear_presence();

    private:
        const char* app_id_;
        bool initialized_ = false;
    };

}
