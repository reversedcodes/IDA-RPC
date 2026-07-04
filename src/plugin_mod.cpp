#include "plugin_mod.hpp"
#include "utils/log/log.hpp"

#include "core/discord/rich_presence.hpp"
#include "core/discord/discord_rpc_helper.hpp"

#include "core/gui/rpc_settings.hpp"
#include "core/gui/settings_dialog.hpp"
#include "core/gui/status_toolbar.hpp"

#include "utils/ida/ida_helper.hpp"

#include <kernwin.hpp>
#include <name.hpp>
#include <funcs.hpp>
#include <loader.hpp>
#include <nalt.hpp>
#include <diskio.hpp>

idarpc::discord::RichPresence *rpc = nullptr;

namespace {
    constexpr int DISCORD_CALLBACK_INTERVAL_MS = 2000;

    int idaapi pump_discord_callbacks(void *) {
        if (rpc != nullptr)
            rpc->run_callbacks();
        idarpc::discord_rpc_helper::tick();
        idarpc::gui::refresh_status();
        return DISCORD_CALLBACK_INTERVAL_MS;
    }
}

void ida_rpc_mod::start_callbacks_timer() {
    if (callbacks_timer_ == nullptr)
        callbacks_timer_ = register_timer(DISCORD_CALLBACK_INTERVAL_MS, pump_discord_callbacks, nullptr);
}

void ida_rpc_mod::stop_callbacks_timer() {
    if (callbacks_timer_ != nullptr) {
        unregister_timer(callbacks_timer_);
        callbacks_timer_ = nullptr;
    }
}

void ida_rpc_mod::init_events() {
    idb_listener = new idarpc::listener::IDBListener();
    hook_event_listener(HT_IDB, idb_listener, 0);

    dbg_listener = new idarpc::listener::DbgListener();
    hook_event_listener(HT_DBG, dbg_listener, 0);


}

void ida_rpc_mod::init_discord_rpc() {
    idarpc::discord_rpc_helper::reload_presence();
}

void ida_rpc_mod::disabled_events() {
    if (idb_listener)
    {
        unhook_event_listener(HT_IDB, idb_listener);
        delete idb_listener;
        idb_listener = nullptr;
    }

    if (ui_listener)
    {
        unhook_event_listener(HT_UI, ui_listener);
        delete ui_listener;
        ui_listener = nullptr;
    }

    if (view_listener)
    {
        unhook_event_listener(HT_VIEW, view_listener);
        delete view_listener;
        view_listener = nullptr;
    }

    if (dbg_listener)
    {
        unhook_event_listener(HT_DBG, dbg_listener);
        delete dbg_listener;
        dbg_listener = nullptr;
    }

}

void ida_rpc_mod::clear_rich_presence() {
    if (rpc)
    {
        rpc->clear_presence();
        delete rpc;
        rpc = nullptr;
    }
}

ida_rpc_mod::ida_rpc_mod()
{
    idarpc::gui::load_settings();
    idarpc::gui::install_settings_menu();
    idarpc::gui::install_status_toolbar();

    init_discord_rpc();
    init_events();
    start_callbacks_timer();
}

ida_rpc_mod::~ida_rpc_mod()
{
    idarpc::log(LogLevel::Warning, "Shutting down...");
    stop_callbacks_timer();
    idarpc::gui::remove_status_toolbar();
    idarpc::gui::remove_settings_menu();
    ida_rpc_mod::disabled_events();
    ida_rpc_mod::clear_rich_presence();
}

bool ida_rpc_mod::run(size_t arg)
{
    idarpc::gui::show_settings_dialog();
    return true;
}
