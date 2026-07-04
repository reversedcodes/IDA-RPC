#include "idb_listener.hpp"
#include "core/discord/discord_rpc_helper.hpp"

ssize_t idaapi idarpc::listener::IDBListener::on_event(ssize_t code, va_list va)
{
    qnotused(va);
    switch (code)
    {
        case idb_event::renamed:
        case idb_event::func_added:
        case idb_event::func_updated:
        case idb_event::make_code:
        case idb_event::make_data:
        case idb_event::cmt_changed:
            idarpc::discord_rpc_helper::mark_activity();
            break;
    }
    return 0;
}

void idarpc::listener::IDBListener::update_presence() {}
