#include "view_listener.hpp"

#include <kernwin.hpp>

namespace idarpc::listener
{
    ssize_t idaapi ViewListener::on_event(ssize_t code, va_list va)
    {
        qnotused(code);
        qnotused(va);
        return 0;
    }
}
