#include "plugin_mod.hpp"
#include "log/log.hpp"
#include "discord/rich_presence.hpp"
#include "helper.hpp"

#include <kernwin.hpp>
#include <name.hpp>
#include <funcs.hpp>
#include <loader.hpp>
#include <nalt.hpp>

idarpc::discord::RichPresence *rpc = nullptr;

static void update_discord_presence()
{
    if (!rpc)
        return;

    rpc->update_presence("IDA Ready", idarpc::helper::get_filename(), "ida_icon", "IDA Pro", "", "");
    //idarpc::log(LogLevel::Info, "Updated Rich Presence.");
}

ida_rpc_mod::ida_rpc_mod()
{
    rpc = new idarpc::discord::RichPresence("1382097836572807199");

    idb_listener = new idarpc::listener::IDBListener();
    hook_event_listener(HT_IDB, idb_listener, 0);

    ui_listener = new idarpc::listener::UIListener();
    hook_event_listener(HT_UI, ui_listener, 0);

    view_listener = new idarpc::listener::ViewListener();
    hook_event_listener(HT_VIEW, view_listener, 0);

    update_discord_presence();
}

ida_rpc_mod::~ida_rpc_mod()
{
    idarpc::log(LogLevel::Warning, "Shutting down...");

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
        unhook_event_listener(HT_UI, view_listener);
        delete view_listener;
        view_listener = nullptr;
    }

    if (rpc)
    {
        rpc->clear_presence();
        delete rpc;
        rpc = nullptr;
    }
}

bool ida_rpc_mod::run(size_t arg)
{
    idarpc::log(LogLevel::Info, "Plugin run() triggered.");
    update_discord_presence();
    return true;
}
