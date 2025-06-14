#include "idb_listener.hpp"
#include "log/log.hpp"

#include <funcs.hpp>
#include <name.hpp>

ssize_t idaapi idarpc::listener::IDBListener::on_event(ssize_t code, va_list va)
{
    switch(code) {
        case idb_event::func_updated:
            break;
        case idb_event::renamed:
            break;
        case idb_event::make_code:
            break;
        case idb_event::make_data:
            break;
    }

    return 0;
}

void idarpc::listener::IDBListener::update_presence() {

}