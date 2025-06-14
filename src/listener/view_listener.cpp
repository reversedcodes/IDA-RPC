#include "view_listener.hpp"
#include "log/log.hpp"
#include <kernwin.hpp>
#include <pro.h>
#include <chrono>

#include "discord_rpc_helper.hpp"

std::string cache_string = "";

namespace idarpc::listener
{

    ssize_t idaapi ViewListener::on_event(ssize_t code, va_list va)
    {

        switch(code) {
            case view_curpos:
                TWidget *view = va_arg(va, TWidget *);
                if(view != nullptr) {
                    qstring title;
                    if(get_widget_title(&title, view)) {
                        update_presence(title.c_str());
                    }                
                }

                break;
        }

        return 0;
    }

    void ViewListener::update_presence(const std::string text) {

        if(text == cache_string) {
            return;
        }
        
        cache_string = text;

        discord_helper_spec spec;
        spec.details = text;
        discord_rpc_helper::update_presence(spec);
    }
}
