#pragma once

#include <ida.hpp>
#include <idp.hpp>
#include <kernwin.hpp>

namespace idarpc::listener {
    class UIListener : public event_listener_t {
    public:
        ssize_t idaapi on_event(ssize_t code, va_list va) override;
    };
}
