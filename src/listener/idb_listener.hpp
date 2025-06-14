#pragma once

#include <ida.hpp>
#include <idp.hpp>
#include <kernwin.hpp>

namespace idarpc::listener {
    class IDBListener : public event_listener_t {
        private:
        void update_presence();

        public:
        ssize_t idaapi on_event(ssize_t code, va_list va) override;
    };
}