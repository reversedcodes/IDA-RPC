#include "ui_listener.hpp"
#include "log/log.hpp"

#include <loader.hpp>     
#include <name.hpp>       
#include <funcs.hpp>     

ssize_t idaapi idarpc::listener::UIListener::on_event(ssize_t code, va_list va)
{
    switch(code) {
        case ui_ready_to_run:
            break;
        case ui_load_file:
            break;
        case ui_refresh:
            break;
        case ui_get_cursor:
            break;
        case ui_get_curline:
            break;
    }

    return 0;
}