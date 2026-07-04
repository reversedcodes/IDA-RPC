#include "dbg_listener.hpp"
#include "core/discord/discord_rpc_helper.hpp"

#include <dbg.hpp>

namespace idarpc::listener
{
    ssize_t idaapi DbgListener::on_event(ssize_t code, va_list va)
    {
        qnotused(va);
        switch (code)
        {
            case dbg_process_start:
            case dbg_process_exit:
            case dbg_process_attach:
            case dbg_process_detach:
            case dbg_suspend_process:
            case dbg_bpt:
            case dbg_step_into:
            case dbg_step_over:
            case dbg_run_to:
                idarpc::discord_rpc_helper::mark_activity();
                break;
        }
        return 0;
    }
}
