#include "view_listener.hpp"
#include "log/log.hpp"
#include "discord/rich_presence.hpp"
#include "helper.hpp"
#include <kernwin.hpp>
#include <chrono>

extern idarpc::discord::RichPresence *rpc;

namespace idarpc::listener
{
    namespace
    {
        std::string last_presence_text;

        bool should_update_presence(const std::string &status_key)
        {

            if (status_key == last_presence_text)
                return false;

            last_presence_text = status_key;
            return true;
        }

    }

    ssize_t idaapi ViewListener::on_event(ssize_t code, va_list va)
    {
        std::string filename = idarpc::helper::get_filename();

        if (code == view_curpos)
        {
            TWidget *view = va_arg(va, TWidget *);
            if (view)
            {
                ea_t ea = get_screen_ea();
                qstring func_name;
                get_func_name(&func_name, ea);
                std::string presence_text = func_name.c_str();

                if (rpc && should_update_presence(presence_text))
                    rpc->update_presence(filename, presence_text, "ida_icon", "IDA Pro", "", "");
            }
        }

        return 0;
    }
}
