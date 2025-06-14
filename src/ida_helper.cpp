#include "ida_helper.hpp"

#include <idp.hpp>
#include <diskio.hpp>

#include "log/log.hpp"

const char *idarpc::idahelper::get_filename()
{
    char filename_buf[260] = {};
    get_root_filename(filename_buf, sizeof(filename_buf));
    return filename_buf;
}

bool idarpc::idahelper::is_ida_home_version()
{
    qstrvec_t dirs;
    get_ida_subdirs(&dirs, nullptr);

    for (const auto &dir : dirs)
    {
        std::string path = dir.c_str();

        path.erase(0, path.find_first_not_of(" \t\r\n"));
        path.erase(path.find_last_not_of(" \t\r\n") + 1);

        std::string clean_path;
        for (char c : path)
        {
            if (c == '\\')
                c = '/';
            if (c != ' ' && c != '\t')
            {
                clean_path += std::tolower(static_cast<unsigned char>(c));
            }
        }

        if (clean_path.find("idahome") != std::string::npos)
        {
            return true;
        }
    }

    return false;
}
