#include "discord_rpc_helper.hpp"
#include "rich_presence.hpp"
#include "utils/ida/ida_helper.hpp"
#include "core/gui/rpc_settings.hpp"
#include "utils/log/log.hpp"

#include <pro.h>
#include <kernwin.hpp>
#include <funcs.hpp>
#include <dbg.hpp>
#include <idp.hpp>
#include <loader.hpp>
#include <name.hpp>

#include <ctime>
#include <string>

extern idarpc::discord::RichPresence *rpc;

namespace
{

    using idarpc::gui::settings;

    time_t g_start_time = time(nullptr);
    time_t g_last_activity = time(nullptr);

    std::string g_last_function;
    std::string g_last_view;
    int g_last_dbg = DSTATE_NOTASK;
    bool g_last_idle = false;
    time_t g_last_push = 0;
    bool g_dirty = true;

    constexpr int MIN_PUSH_INTERVAL_SEC = 5;

    std::string ida_version_string()
    {
        char version[32] = {0};
        get_kernel_version(version, sizeof(version));
        return std::string(version);
    }

    std::string current_function()
    {
        const ea_t ea = get_screen_ea();
        if (ea == BADADDR)
            return "";
        qstring name;
        if (get_func_name(&name, ea) > 0)
            return name.c_str();
        return "";
    }

    struct ViewInfo
    {
        std::string name;
        std::string key;
    };

    ViewInfo current_view()
    {
        TWidget *w = get_current_widget();
        if (w == nullptr)
            return {"", ""};

        switch (static_cast<int>(get_widget_type(w)))
        {
        case BWN_DISASM:
            return {"Disassembly", "disasm"};
        case BWN_PSEUDOCODE:
            return {"Pseudocode", "pseudocode"};
        case BWN_HEXVIEW:
            return {"Hex View", "hex"};
        case BWN_STRINGS:
            return {"Strings", "strings"};
        case BWN_IMPORTS:
            return {"Imports", "imports"};
        case BWN_EXPORTS:
            return {"Exports", "exports"};
        case BWN_NAMES:
            return {"Names", "names"};
        case BWN_FUNCS:
            return {"Functions", "functions"};
        case BWN_SEGS:
            return {"Segments", "segments"};
        default:
            return {"", ""};
        }
    }

    std::string file_display()
    {
        if (settings().hide_file_name)
            return "a binary";
        return idarpc::idahelper::get_filename();
    }

    std::string address_string()
    {
        const ea_t ea = get_screen_ea();
        if (ea == BADADDR)
            return "";
        char buf[32] = {0};
        qsnprintf(buf, sizeof(buf), "0x%llX", static_cast<unsigned long long>(ea));
        return buf;
    }

    void replace_all(std::string &s, const std::string &from, const std::string &to)
    {
        if (from.empty())
            return;
        size_t pos = 0;
        while ((pos = s.find(from, pos)) != std::string::npos)
        {
            s.replace(pos, from.size(), to);
            pos += to.size();
        }
    }

    std::string cleanup(const std::string &in)
    {
        std::string out;
        bool prev_space = false;
        for (char c : in)
        {
            if (c == ' ' || c == '\t')
            {
                if (!out.empty() && !prev_space)
                    out += ' ';
                prev_space = true;
            }
            else
            {
                out += c;
                prev_space = false;
            }
        }
        while (!out.empty() && (out.back() == ' ' || out.back() == '-'))
            out.pop_back();
        while (!out.empty() && (out.front() == ' ' || out.front() == '-'))
            out.erase(out.begin());
        return out;
    }

    std::string expand_template(const std::string &tmpl)
    {
        const idarpc::gui::RpcSettings &s = settings();
        std::string out = tmpl;
        replace_all(out, "{function}", s.show_function ? current_function() : "");
        replace_all(out, "{view}", s.show_view ? current_view().name : "");
        replace_all(out, "{file}", s.show_file ? file_display() : "");
        replace_all(out, "{address}", address_string());
        replace_all(out, "{version}", s.show_ida_version ? ida_version_string() : "");
        return cleanup(out);
    }

    bool is_idle_now()
    {
        const idarpc::gui::RpcSettings &s = settings();
        if (!s.idle_enabled)
            return false;
        if (get_process_state() != DSTATE_NOTASK)
            return false;
        return (time(nullptr) - g_last_activity) * 1000 >= static_cast<long long>(s.idle_timeout_ms);
    }

}

namespace idarpc::discord_rpc_helper
{
    void mark_activity()
    {
        g_last_activity = time(nullptr);
        g_dirty = true;
    }

    void render_presence()
    {
        if (rpc == nullptr)
            return;

        const idarpc::gui::RpcSettings &s = idarpc::gui::settings();

        DiscordRichPresence presence{};

        if (s.show_elapsed)
            presence.startTimestamp = g_start_time;

        const bool home = idarpc::idahelper::is_ida_home_version();
        presence.largeImageKey = home ? "ida_home" : "ida_pro";

        std::string large_text, details, state_line, small_text;

        if (s.show_ida_version)
        {
            large_text = (home ? "IDA HOME " : "IDA PRO ") + ida_version_string();
            presence.largeImageText = large_text.c_str();
        }

        const int dbg = get_process_state();
        const ViewInfo view = current_view();

        if (s.show_debugging && dbg != DSTATE_NOTASK)
        {
            details = "Debugging " + file_display();
            state_line = (dbg == DSTATE_SUSP) ? "Paused" : "Running";
        }
        else if (is_idle_now())
        {
            details = "Idle";
        }
        else
        {
            details = expand_template(s.details_template);
            state_line = expand_template(s.state_template);
        }

        if (!details.empty())
            presence.details = details.c_str();
        if (!state_line.empty())
            presence.state = state_line.c_str();

        if (s.show_small_image && !view.key.empty())
        {
            presence.smallImageKey = view.key.c_str();
            small_text = view.name;
            presence.smallImageText = small_text.c_str();
        }

        rpc->update_presence(presence);
        g_last_push = time(nullptr);
        g_dirty = false;
    }

    void tick()
    {
        if (rpc == nullptr)
            return;

        const std::string fn = current_function();
        const std::string view = current_view().name;
        if (fn != g_last_function || view != g_last_view)
        {
            g_last_function = fn;
            g_last_view = view;
            mark_activity();
        }

        const int dbg = get_process_state();
        if (dbg != g_last_dbg)
        {
            g_last_dbg = dbg;
            mark_activity();
        }

        const bool idle = is_idle_now();
        if (idle != g_last_idle)
        {
            g_last_idle = idle;
            g_dirty = true;
        }

        if (!g_dirty)
            return;

        if (time(nullptr) - g_last_push < MIN_PUSH_INTERVAL_SEC)
            return;

        render_presence();
    }

    void reload_presence()
    {
        if (rpc)
        {
            rpc->clear_presence();
            delete rpc;
            rpc = nullptr;
        }

        if (!idarpc::gui::settings().enabled)
            return;

        rpc = new discord::RichPresence(idarpc::gui::settings().client_id.c_str());
        g_dirty = true;
        render_presence();
    }

    void reconnect()
    {
        if (!idarpc::gui::settings().enabled)
        {
            idarpc::log(LogLevel::Warning, "Cannot reconnect: Rich Presence is disabled.");
            return;
        }

        if (rpc != nullptr)
            rpc->reconnect();
        else
            reload_presence();

        render_presence();
    }

    bool is_connected()
    {
        return rpc != nullptr && discord::RichPresence::is_connected();
    }

    std::string status_text()
    {
        if (!idarpc::gui::settings().enabled)
            return "Disabled";
        if (rpc == nullptr)
            return "Not connected";
        if (discord::RichPresence::is_connected())
        {
            const std::string user = discord::RichPresence::connected_username();
            return user.empty() ? "Connected to Discord" : ("Connected to Discord as " + user);
        }
        return "Connecting...";
    }

}
