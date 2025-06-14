#include "rich_presence.hpp"
#include "log/log.hpp"

namespace idarpc::discord {

    RichPresence::RichPresence(const char* app_id) : app_id_(app_id) {
        DiscordEventHandlers handlers{};
        Discord_Initialize(app_id_, &handlers, 1, nullptr);
        initialized_ = true;
        idarpc::log(LogLevel::Info, "Initializing Rich Presence...");

    }

    RichPresence::~RichPresence() {
        if (initialized_) {
            Discord_Shutdown();
            idarpc::log(LogLevel::Warning, "Shutdown Rich Presence.");
        }
    }

    void RichPresence::update_presence(DiscordRichPresence rpc) {
        if (!initialized_) 
            return;

        Discord_UpdatePresence(&rpc);
    }

    void RichPresence::clear_presence() {
        if (initialized_) {
            Discord_ClearPresence();
            idarpc::log(LogLevel::Info, "Presence cleared.");
        }
    }
} 
