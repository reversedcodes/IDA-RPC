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

        void run_callbacks();

        void reconnect();

        static bool is_connected();
        static const char *connected_username();

    private:
        void initialize();

        std::string app_id_;
        bool initialized_ = false;
    };

}
