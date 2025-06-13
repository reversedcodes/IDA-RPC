#pragma once

#include <string>
#include <discord_rpc.h>

namespace idarpc::discord {

    class RichPresence {
    public:
        explicit RichPresence(const char* app_id);
        ~RichPresence();

        void update_presence(
            const std::string& state,
            const std::string& details = "",
            const std::string& large_image_key = "",
            const std::string& large_image_text = "",
            const std::string& small_image_key = "",
            const std::string& small_image_text = ""
        );

        void clear_presence();

    private:
        const char* app_id_;
        bool initialized_ = false;
    };

}
