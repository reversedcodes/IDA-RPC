#include "status_toolbar.hpp"

#include "utils/log/log.hpp"
#include "core/discord/discord_rpc_helper.hpp"

#include <ida.hpp>
#include <kernwin.hpp>

#include <string>


namespace idarpc::gui {

    namespace {

        constexpr char RECONNECT_NAME[] = "idarpc:reconnect";
        constexpr char RECONNECT_LABEL[] = "Reconnect to Discord";
        constexpr char RECONNECT_TOOLTIP[] = "Reconnect the Discord Rich Presence";
        constexpr char MENU_PATH[] = "Discord/";

        int g_refs = 0;
        bool g_last_connected = false;
        bool g_have_state = false;

        struct reconnect_action_handler_t : public action_handler_t
        {
            int idaapi activate(action_activation_ctx_t *) override
            {
                idarpc::discord_rpc_helper::reconnect();
                refresh_status();
                return 1;
            }

            action_state_t idaapi update(action_update_ctx_t *) override
            {
                return AST_ENABLE_ALWAYS;
            }
        };

        reconnect_action_handler_t g_handler;

    }

    void install_status_toolbar()
    {
        if (g_refs++ > 0)
            return;

        const action_desc_t desc = ACTION_DESC_LITERAL_OWNER(
            RECONNECT_NAME, RECONNECT_LABEL, &g_handler, nullptr,
            nullptr, RECONNECT_TOOLTIP, -1, 0);

        if (!register_action(desc))
        {
            idarpc::log(LogLevel::Error, "Failed to register the reconnect action.");
            g_refs = 0;
            return;
        }

        attach_action_to_menu(MENU_PATH, RECONNECT_NAME, SETMENU_APP);

        g_have_state = false;
        refresh_status();
    }

    void remove_status_toolbar()
    {
        if (g_refs == 0 || --g_refs > 0)
            return;

        detach_action_from_menu(MENU_PATH, RECONNECT_NAME);
        unregister_action(RECONNECT_NAME);
        g_have_state = false;
    }

    void refresh_status()
    {
        if (g_refs == 0)
            return;

        const bool connected = idarpc::discord_rpc_helper::is_connected();
        if (g_have_state && connected == g_last_connected)
            return;

        g_last_connected = connected;
        g_have_state = true;

        std::string tip = idarpc::discord_rpc_helper::status_text();
        if (!connected)
            tip += " - click to reconnect";
        update_action_tooltip(RECONNECT_NAME, tip.c_str());
    }

}
