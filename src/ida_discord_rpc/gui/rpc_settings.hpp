#pragma once

#include <string>

namespace idarpc::gui
{
    struct RpcSettings
    {
        bool enabled = true;
        std::string client_id = "1383503123666173952";

        bool show_function = true;
        bool show_view = true;
        bool show_file = true;
        bool show_ida_version = true;
        bool show_elapsed = true;
        bool show_small_image = true;

        bool show_debugging = true;

        bool idle_enabled = true;
        int idle_timeout_ms = 300000;

        bool hide_file_name = false;

        std::string details_template = "{function}";
        std::string state_template = "{view} - {file}";
    };

    RpcSettings &settings();
    bool load_settings();
    bool save_settings();

}
