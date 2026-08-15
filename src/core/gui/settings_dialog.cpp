#include "settings_dialog.hpp"
#include "rpc_settings.hpp"

#include "utils/log/log.hpp"
#include "core/discord/discord_rpc_helper.hpp"

#include <ida.hpp>
#include <kernwin.hpp>

#include <cstdlib>
#include <string>

namespace idarpc::gui
{

    namespace
    {

        constexpr char ACTION_NAME[] = "idarpc:settings";
        constexpr char ACTION_LABEL[] = "Settings...";
        constexpr char ACTION_TOOLTIP[] = "Configure Discord Rich Presence";

        constexpr char MENU_NAME[] = "Discord";
        constexpr char MENU_LABEL[] = "Discord";
        constexpr char MENU_ANCHOR[] = "Help";
        constexpr char MENU_PATH[] = "Discord/";
        int g_menu_refs = 0;

        struct settings_action_handler_t : public action_handler_t
        {
            int idaapi activate(action_activation_ctx_t *) override
            {
                show_settings_dialog();
                return 1;
            }

            action_state_t idaapi update(action_update_ctx_t *) override
            {
                return AST_ENABLE_ALWAYS;
            }
        };

        // The kernel destroys the handler when the action is unregistered, so it
        // has to be allocated on the heap (action_handler_t routes new/delete
        // through qalloc/qfree). A static instance would make the kernel call
        // qfree() on a .bss address and abort the process.
        settings_action_handler_t *g_handler = nullptr;

        int idaapi reset_settings_cb(int, form_actions_t &fa)
        {
            settings() = RpcSettings{};
            save_settings();
            idarpc::discord_rpc_helper::reload_presence();
            fa.close(0);
            return 0;
        }

    }

    void show_settings_dialog()
    {
        const std::string form =
            "STARTITEM 0\n"
            "Discord Rich Presence\n"
            "\n"
            "Status: " + idarpc::discord_rpc_helper::status_text() + "\n"
            "\n"
            "<#Enable or disable Discord Rich Presence integration#"
            "~E~nable Rich Presence:C>>\n"
            "\n"
            "<#Discord application (client) ID from the Developer Portal#"
            "Client ~I~D:q:64:48::>\n"
            "\n"
            "Show:\n"
            "<#Show the current function name#Function:C>\n"
            "<#Show the current view type (Disassembly, Pseudocode, ...)#View type:C>\n"
            "<#Show the analyzed file name#File name:C>\n"
            "<#Show the IDA edition and version#IDA version:C>\n"
            "<#Show the elapsed time#Elapsed time:C>\n"
            "<#Show a small activity icon for the current view#Activity icon:C>>\n"
            "\n"
            "Features:\n"
            "<#Show 'Debugging <file>' while a process is being debugged#Debugging status:C>\n"
            "<#Show 'Idle' after a period of inactivity#Idle detection:C>\n"
            "<#Show 'a binary' instead of the real file name#Hide file name (privacy):C>>\n"
            "\n"
            "<#Milliseconds of inactivity before showing Idle#Idle timeout (ms):q:12:12::>\n"
            "\n"
            "Templates - placeholders: {function} {view} {file} {address} {version}\n"
            "<#Format of the details line#Details format:q:128:48::>\n"
            "<#Format of the state line#State format:q:128:48::>\n"
            "\n"
            "<~R~eset all settings to defaults:B20:0:::>\n"
            "\n";

        RpcSettings &s = settings();
        const bool was_enabled = s.enabled;
        const std::string old_client = s.client_id;

        ushort enabled = s.enabled ? 1 : 0;
        qstring client_id = s.client_id.c_str();
        ushort display =
            (s.show_function ? 1u << 0 : 0) |
            (s.show_view ? 1u << 1 : 0) |
            (s.show_file ? 1u << 2 : 0) |
            (s.show_ida_version ? 1u << 3 : 0) |
            (s.show_elapsed ? 1u << 4 : 0) |
            (s.show_small_image ? 1u << 5 : 0);
        ushort features =
            (s.show_debugging ? 1u << 0 : 0) |
            (s.idle_enabled ? 1u << 1 : 0) |
            (s.hide_file_name ? 1u << 2 : 0);
        qstring idle_str;
        idle_str.sprnt("%d", s.idle_timeout_ms);
        qstring details_tmpl = s.details_template.c_str();
        qstring state_tmpl = s.state_template.c_str();

        if (ask_form(form.c_str(), &enabled, &client_id, &display, &features,
                     &idle_str, &details_tmpl, &state_tmpl, reset_settings_cb) <= 0)
            return;

        s.enabled = (enabled & 1) != 0;
        s.client_id = client_id.c_str();
        s.show_function = (display & (1u << 0)) != 0;
        s.show_view = (display & (1u << 1)) != 0;
        s.show_file = (display & (1u << 2)) != 0;
        s.show_ida_version = (display & (1u << 3)) != 0;
        s.show_elapsed = (display & (1u << 4)) != 0;
        s.show_small_image = (display & (1u << 5)) != 0;
        s.show_debugging = (features & (1u << 0)) != 0;
        s.idle_enabled = (features & (1u << 1)) != 0;
        s.hide_file_name = (features & (1u << 2)) != 0;
        int ms = atoi(idle_str.c_str());
        s.idle_timeout_ms = ms > 0 ? ms : 300000;
        s.details_template = details_tmpl.c_str();
        s.state_template = state_tmpl.c_str();

        if (!save_settings())
            warning("Discord RPC: could not save the settings file.");
        if (s.enabled != was_enabled || s.client_id != old_client)
            idarpc::discord_rpc_helper::reload_presence();
        else
            idarpc::discord_rpc_helper::render_presence();
        idarpc::log(LogLevel::Info, "Settings updated.");
    }

    void install_settings_menu()
    {
        if (g_menu_refs++ > 0)
            return;

        create_menu(MENU_NAME, MENU_LABEL, MENU_ANCHOR);

        g_handler = new settings_action_handler_t();

        const action_desc_t settings_desc = ACTION_DESC_LITERAL_OWNER(
            ACTION_NAME, ACTION_LABEL, g_handler, nullptr,
            nullptr, ACTION_TOOLTIP, -1, 0);

        if (register_action(settings_desc))
        {
            attach_action_to_menu(MENU_PATH, ACTION_NAME, SETMENU_APP);
        }
        else
        {
            delete g_handler;
            g_handler = nullptr;
            idarpc::log(LogLevel::Error, "Failed to register the settings action.");
        }
    }

    void remove_settings_menu()
    {
        if (g_menu_refs == 0 || --g_menu_refs > 0)
            return;

        detach_action_from_menu(MENU_PATH, ACTION_NAME);
        unregister_action(ACTION_NAME); // destroys g_handler
        g_handler = nullptr;
        delete_menu(MENU_NAME);
    }

}
