#include "rpc_settings.hpp"
#include "utils/log/log.hpp"

#include <diskio.hpp>

#include <fstream>
#include <string>

namespace idarpc::gui
{

    namespace
    {
        constexpr char CONFIG_FILENAME[] = "discord_rpc.cfg";
        std::string config_path()
        {
            const char *dir = get_user_idadir();
            std::string path = (dir != nullptr && dir[0] != '\0') ? dir : ".";
            if (path.back() != '/' && path.back() != '\\')
                path += '/';
            path += CONFIG_FILENAME;
            return path;
        }

        std::string trim(const std::string &s)
        {
            constexpr char ws[] = " \t\r\n";
            const size_t begin = s.find_first_not_of(ws);
            if (begin == std::string::npos)
                return "";
            const size_t end = s.find_last_not_of(ws);
            return s.substr(begin, end - begin + 1);
        }

        bool to_bool(const std::string &v)
        {
            return v == "1" || v == "true";
        }

        int to_int(const std::string &v, int fallback)
        {
            try
            {
                return std::stoi(v);
            }
            catch (...)
            {
                return fallback;
            }
        }

    }

    RpcSettings &settings()
    {
        static RpcSettings instance;
        return instance;
    }

    bool load_settings()
    {
        std::ifstream in(config_path());
        if (!in.is_open())
            return false;

        std::string line;
        while (std::getline(in, line))
        {
            const size_t eq = line.find('=');
            if (eq == std::string::npos)
                continue;

            const std::string key = trim(line.substr(0, eq));
            const std::string value = trim(line.substr(eq + 1));

            if (key == "enabled")
                settings().enabled = to_bool(value);
            else if (key == "client_id")
                settings().client_id = value;
            else if (key == "show_function")
                settings().show_function = to_bool(value);
            else if (key == "show_view")
                settings().show_view = to_bool(value);
            else if (key == "show_file")
                settings().show_file = to_bool(value);
            else if (key == "show_ida_version")
                settings().show_ida_version = to_bool(value);
            else if (key == "show_elapsed")
                settings().show_elapsed = to_bool(value);
            else if (key == "show_small_image")
                settings().show_small_image = to_bool(value);
            else if (key == "show_debugging")
                settings().show_debugging = to_bool(value);
            else if (key == "idle_enabled")
                settings().idle_enabled = to_bool(value);
            else if (key == "idle_timeout_ms")
                settings().idle_timeout_ms = to_int(value, settings().idle_timeout_ms);
            else if (key == "hide_file_name")
                settings().hide_file_name = to_bool(value);
            else if (key == "details_template")
                settings().details_template = value;
            else if (key == "state_template")
                settings().state_template = value;
        }
        return true;
    }

    bool save_settings()
    {
        std::ofstream out(config_path(), std::ios::trunc);
        if (!out.is_open())
        {
            idarpc::log(LogLevel::Error, "Could not open settings file for writing.");
            return false;
        }

        const RpcSettings &s = settings();
        out << "enabled=" << (s.enabled ? 1 : 0) << "\n";
        out << "client_id=" << s.client_id << "\n";
        out << "show_function=" << (s.show_function ? 1 : 0) << "\n";
        out << "show_view=" << (s.show_view ? 1 : 0) << "\n";
        out << "show_file=" << (s.show_file ? 1 : 0) << "\n";
        out << "show_ida_version=" << (s.show_ida_version ? 1 : 0) << "\n";
        out << "show_elapsed=" << (s.show_elapsed ? 1 : 0) << "\n";
        out << "show_small_image=" << (s.show_small_image ? 1 : 0) << "\n";
        out << "show_debugging=" << (s.show_debugging ? 1 : 0) << "\n";
        out << "idle_enabled=" << (s.idle_enabled ? 1 : 0) << "\n";
        out << "idle_timeout_ms=" << s.idle_timeout_ms << "\n";
        out << "hide_file_name=" << (s.hide_file_name ? 1 : 0) << "\n";
        out << "details_template=" << s.details_template << "\n";
        out << "state_template=" << s.state_template << "\n";
        return true;
    }

}
