#pragma once

#include <ida.hpp>
#include <idp.hpp>
#include <kernwin.hpp>

#include "core/listener/ui_listener.hpp"
#include "core/listener/idb_listener.hpp"
#include "core/listener/view_listener.hpp"
#include "core/listener/dbg_listener.hpp"

class ida_rpc_mod : public plugmod_t {
     idarpc::listener::UIListener* ui_listener = nullptr;
     idarpc::listener::IDBListener* idb_listener = nullptr;
     idarpc::listener::ViewListener* view_listener = nullptr;
     idarpc::listener::DbgListener* dbg_listener = nullptr;

     qtimer_t callbacks_timer_ = nullptr;

private:
    void init_discord_rpc();
    void init_events();
    void clear_rich_presence();
    void disabled_events();

    void start_callbacks_timer();
    void stop_callbacks_timer();

public:
    ida_rpc_mod();
    virtual ~ida_rpc_mod();
    virtual bool idaapi run(size_t arg) override;
};
