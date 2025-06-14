#include "view_listener.hpp"
#include "log/log.hpp"
#include <kernwin.hpp>
#include <chrono>

namespace idarpc::listener
{

    ssize_t idaapi ViewListener::on_event(ssize_t code, va_list va)
    {

        switch(code) {
            case view_curpos:
                break;
        }

        return 0;
    }
}
