#include "idb_listener.hpp"
#include "log/log.hpp"
#include "discord/rich_presence.hpp"
#include "helper.hpp"

#include <funcs.hpp>
#include <name.hpp>

extern idarpc::discord::RichPresence *rpc;

ssize_t idaapi idarpc::listener::IDBListener::on_event(ssize_t code, va_list va)
{
    std::string filename = idarpc::helper::get_filename();

    switch (code)
    {
    case idb_event::func_updated:
    {
        func_t *pfn = va_arg(va, func_t *);
        if (pfn != nullptr)
        {
            std::string details = "Func updated: 0x" + std::to_string(pfn->start_ea);

            if (rpc)
                rpc->update_presence(filename, details, "ida_icon", "IDA Pro", "", "");
        }
        break;
    }

    case idb_event::renamed:
    {
        va_arg(va, ea_t);
        const char *new_name = va_arg(va, const char *);
        va_arg(va, bool);
        const char *old_name = va_arg(va, const char *);

        std::string details = "Renamed: " + std::string(old_name ? old_name : "<none>") + " -> " + std::string(new_name ? new_name : "<none>");

        if (rpc)
            rpc->update_presence(filename, details, "ida_icon", "IDA Pro", "", "");

        break;
    }

    case idb_event::make_code:
    {
        const insn_t *insn = va_arg(va, const insn_t *);
        if (insn)
        {
            std::string details = "Make code: 0x" + std::to_string(insn->ea);
            if (rpc)
                rpc->update_presence(filename, details, "ida_icon", "IDA Pro", "", "");
        }
        break;
    }

    case idb_event::make_data:
    {
        ea_t ea = va_arg(va, ea_t);
        va_arg(va, flags64_t);
        va_arg(va, tid_t);
        asize_t len = va_arg(va, asize_t);

        std::string details = "Data created: 0x" + std::to_string(ea) + " (len: " + std::to_string(len) + ")";

        if (rpc)
            rpc->update_presence(filename, details, "ida_icon", "IDA Pro", "", "");

        break;
    }

    default:
        break;
    }

    return 0;
}