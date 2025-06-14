#pragma once

#include <ida.hpp>
#include <idp.hpp>
#include <kernwin.hpp>

#include "listener/ui_listener.hpp"
#include "listener/idb_listener.hpp"
#include "listener/view_listener.hpp"

class ida_rpc_mod : public plugmod_t {
     idarpc::listener::UIListener* ui_listener = nullptr;
     idarpc::listener::IDBListener* idb_listener = nullptr;
     idarpc::listener::ViewListener* view_listener = nullptr;

private:
    void init_discord_rpc();
    void init_events();
    void clear_rich_presence();
    void disabled_events();

public:
    ida_rpc_mod();
    virtual ~ida_rpc_mod();
    virtual bool idaapi run(size_t arg) override;
};
