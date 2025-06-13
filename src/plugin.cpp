#include <ida.hpp>
#include <idp.hpp>
#include <loader.hpp>

#include "plugin_mod.hpp"

static plugmod_t* idaapi init(void)
{
    return new ida_rpc_mod();
}

plugin_t PLUGIN = {
    IDP_INTERFACE_VERSION,
    PLUGIN_MOD,
    init,
    nullptr,
    nullptr,
    "Adds Discord Rich Presence integration for IDA.",
    "Discord Rich Presence",
    "IDA Discord RPC",
    "Created by Reversed Codes",
};
