#include "rich_presence.hpp"
#include "utils/log/log.hpp"

#include <string>

namespace idarpc::discord {

    namespace {
        bool g_connected = false;
        std::string g_username;

        void on_ready(const DiscordUser *user) {
            g_connected = true;
            g_username = (user != nullptr && user->username != nullptr) ? user->username : "";
            idarpc::log(LogLevel::Info,
                        "Connected to Discord as " + (g_username.empty() ? std::string("?") : g_username));
        }

        void on_disconnected(int error_code, const char *message) {
            g_connected = false;
            idarpc::log(LogLevel::Warning,
                        "Disconnected from Discord (" + std::to_string(error_code) + "): " +
                            (message != nullptr ? message : ""));
        }

        void on_errored(int error_code, const char *message) {
            g_connected = false;
            idarpc::log(LogLevel::Error,
                        "Discord error (" + std::to_string(error_code) + "): " +
                            (message != nullptr ? message : ""));
        }
    }

    void RichPresence::initialize() {
        DiscordEventHandlers handlers{};
        handlers.ready = on_ready;
        handlers.disconnected = on_disconnected;
        handlers.errored = on_errored;
        Discord_Initialize(app_id_.c_str(), &handlers, 1, nullptr);
        initialized_ = true;
    }

    RichPresence::RichPresence(const char* app_id) : app_id_(app_id != nullptr ? app_id : "") {
        initialize();
        idarpc::log(LogLevel::Info, "Initializing Rich Presence...");
    }

    RichPresence::~RichPresence() {
        if (initialized_) {
            Discord_Shutdown();
            g_connected = false;
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

    void RichPresence::run_callbacks() {
        if (initialized_)
            Discord_RunCallbacks();
    }

    void RichPresence::reconnect() {
        idarpc::log(LogLevel::Info, "Reconnecting to Discord...");
        if (initialized_) {
            Discord_Shutdown();
            initialized_ = false;
        }
        g_connected = false;
        initialize();
    }

    bool RichPresence::is_connected() {
        return g_connected;
    }

    const char *RichPresence::connected_username() {
        return g_username.c_str();
    }
}
