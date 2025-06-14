#include "plugin_mod.hpp"
#include "log/log.hpp"

#include "discord/rich_presence.hpp"
#include "discord_rpc_helper.hpp"

#include "ida_helper.hpp"

#include <kernwin.hpp>
#include <name.hpp>
#include <funcs.hpp>
#include <loader.hpp>
#include <nalt.hpp>
#include <diskio.hpp>

idarpc::discord::RichPresence *rpc = nullptr;

void ida_rpc_mod::init_events() {
    //idb_listener = new idarpc::listener::IDBListener();
    //hook_event_listener(HT_IDB, idb_listener, 0);

    //ui_listener = new idarpc::listener::UIListener();
    //hook_event_listener(HT_UI, ui_listener, 0);

    view_listener = new idarpc::listener::ViewListener();
    hook_event_listener(HT_VIEW, view_listener, 0);
}

void ida_rpc_mod::init_discord_rpc() {
    rpc = new idarpc::discord::RichPresence("1383503123666173952");
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
        unhook_event_listener(HT_UI, view_listener);
        delete view_listener;
        view_listener = nullptr;
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

//Create
ida_rpc_mod::ida_rpc_mod()
{

    init_discord_rpc();
    init_events();

    discord_helper_spec spec;
    spec.details = "Hello World";

    idarpc::discord_rpc_helper::update_presence(spec);
}

//Close
ida_rpc_mod::~ida_rpc_mod()
{
    idarpc::log(LogLevel::Warning, "Shutting down...");
    ida_rpc_mod::disabled_events();
    ida_rpc_mod::clear_rich_presence();
}

bool ida_rpc_mod::run(size_t arg)
{
    idarpc::log(LogLevel::Info, "Plugin run() triggered.");
    return true;
}
