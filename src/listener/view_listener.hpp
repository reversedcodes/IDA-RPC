#pragma once

#include <ida.hpp>
#include <idp.hpp>

namespace idarpc::listener {
    class ViewListener : public event_listener_t {
    private:
        void update_presence(const std::string text);
    public:
        ssize_t idaapi on_event(ssize_t code, va_list va) override;
    };
}