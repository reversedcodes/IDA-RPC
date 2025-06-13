#include "helper.hpp"

#include <idp.hpp>
#include "log/log.hpp"

const char* idarpc::helper::get_filename() {
    char filename_buf[260] = {};
    get_root_filename(filename_buf, sizeof(filename_buf));
    return filename_buf;
}
