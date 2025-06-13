#include "rich_presence.hpp"
#include "log/log.hpp"

namespace idarpc::discord {

    RichPresence::RichPresence(const char* app_id) : app_id_(app_id) {
        DiscordEventHandlers handlers{};
        Discord_Initialize(app_id_, &handlers, 1, nullptr);
        initialized_ = true;
        idarpc::log(LogLevel::Info, "Initialized Rich Presence.");
    }

    RichPresence::~RichPresence() {
        if (initialized_) {
            Discord_Shutdown();
            idarpc::log(LogLevel::Warning, "Shutdown Rich Presence.");
        }
    }

    void RichPresence::update_presence(
        const std::string& state,
        const std::string& details,
        const std::string& large_image_key,
        const std::string& large_image_text,
        const std::string& small_image_key,
        const std::string& small_image_text
    ) {
        if (!initialized_) 
            return;

        DiscordRichPresence presence{};
        presence.state = state.c_str();
        presence.details = details.c_str();

        if (!large_image_key.empty())
            presence.largeImageKey = large_image_key.c_str();
        if (!large_image_text.empty())
            presence.largeImageText = large_image_text.c_str();

        if (!small_image_key.empty())
            presence.smallImageKey = small_image_key.c_str();
        if (!small_image_text.empty())
            presence.smallImageText = small_image_text.c_str();

        Discord_UpdatePresence(&presence);
        idarpc::log(LogLevel::Info, "Presence updated.");
    }

    void RichPresence::clear_presence() {
        if (initialized_) {
            Discord_ClearPresence();
            idarpc::log(LogLevel::Info, "Presence cleared.");
        }
    }
} 
