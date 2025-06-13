#include "ui_listener.hpp"
#include "log/log.hpp"
#include "discord/rich_presence.hpp"
#include "helper.hpp"

#include <loader.hpp>     
#include <name.hpp>       
#include <funcs.hpp>     

extern idarpc::discord::RichPresence *rpc;

ssize_t idaapi idarpc::listener::UIListener::on_event(ssize_t code, va_list va)
{
    std::string filename = idarpc::helper::get_filename();

    switch (code)
    {
    case ui_ready_to_run:
    {
        std::string status = "IDA is ready.";

        if (rpc)
            rpc->update_presence(filename, status, "ida_icon", "IDA Pro", "", "");

        idarpc::log(LogLevel::Info, filename + " | " + status);
        break;
    }

    case ui_load_file:
    case ui_refresh:
    case ui_get_cursor:
    case ui_get_curline:
    {
        TWidget* widget = va_arg(va, TWidget*);

        if (widget != nullptr)
        {
            qstring title;
            if (get_widget_title(&title, widget))
            {
                ea_t ea = get_screen_ea();
                qstring func_name;

                if (!get_func_name(&func_name, ea))
                    func_name = "<unknown>";

                if (rpc)
                    rpc->update_presence(filename, std::string(title.c_str()), "ida_icon", "IDA Pro", "", "");

                idarpc::log(LogLevel::Info, filename);
            }
        }
        break;
    }

    default:
        break;
    }

    return 0;
}